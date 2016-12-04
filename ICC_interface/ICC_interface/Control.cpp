#include "stdafx.h"
#include "ControlWindow.h"
#include "OptionsWindow.h"
#include "StatisticsWindow.h"
#include "SnapshotWindow.h"

namespace ICC_interface
{
	const wchar_t* settingFile = L"settings.bin";
	namespace golfTransmissionConsts
	{
		const int maximgwidth = 2048;
		const int maximgheight = 1088;
		const int imgsOffset1 = sizeof(golfTransmission);
		const int imgsOffset2 = imgsOffset1 + maximgwidth*maximgheight*3;
		const int imgsOffset3 = imgsOffset2 + maximgwidth*maximgheight*3;
		const int imgsOffset4 = imgsOffset3 + maximgwidth*maximgheight*3;
		const int fileSize = imgsOffset4 + maximgwidth*maximgheight*3;
		
		const int optionsOffset = offsetof(golfTransmission,sift_thresh);
		const int optionsSize = imgsOffset1-optionsOffset;

		const int controlsOffset = offsetof(golfTransmission,render_img1);
		const int controlsSize = optionsOffset-controlsOffset;

		const int imgsizeoffset = offsetof(golfTransmission, img_width1);
		const int imgsizesize = optionsOffset-imgsizeoffset;
	}

	void LoadImage(unsigned char* ptr,  PictureBox^ box, int offset, int width, int height)
	{
		Imaging::PixelFormat format = Imaging::PixelFormat::Format24bppRgb;
		Rectangle rect(0,0,width,height);
			
		Bitmap^ bitmap = gcnew Bitmap(width,height,format);
		Imaging::BitmapData^ data = bitmap->LockBits(rect,Imaging::ImageLockMode::WriteOnly,bitmap->PixelFormat);
		
		memcpy((void*)data->Scan0, ptr+offset, width*height*3);

		bitmap->UnlockBits(data);

		box->Image = bitmap;
	}

	ControlWindow::ControlWindow(void)
	{
		InitializeComponent();

		statisticsForm = gcnew StatisticsWindow();
		optionsForm = gcnew OptionsWindow();
		newballForm = gcnew SnapshotWindow();

		golfData = new golfTransmission;
		optionsForm->golfData = golfData;

		filePipeName = L"golfDataPipe";
		mutexName = L"golfDataMutex";

		try{
			FileStream^ datafile = File::Open(gcnew String(settingFile), FileMode::OpenOrCreate);
			//if(datafile->Length<golfTransmissionConsts::optionsSize)
			//{
			//	golfData->setDefaults();
			//}
			//else
			{
				array<Byte>^ bytes = gcnew array<Byte>(golfTransmissionConsts::optionsSize);
				datafile->Read(bytes, 0, golfTransmissionConsts::optionsSize);
				IntPtr dst((char*)golfData+golfTransmissionConsts::optionsOffset);
				System::Runtime::InteropServices::Marshal::Copy(bytes, 0, dst, golfTransmissionConsts::optionsSize);
			}
			datafile->Close();
		}
		catch(Exception^ e){MessageBox::Show(this,L"Error reading options file: " + e->Message); golfData->setDefaults();}
	}

	bool ControlWindow::Init()
	{
		try{filePipe = MemoryMappedFile::CreateNew(filePipeName, golfTransmissionConsts::fileSize, MemoryMappedFileAccess::ReadWrite);}
		catch(Exception^)
		{MessageBox::Show(this,L"Memory mapped file is still in use. Make sure all instances of this are closed."); return false;}
		mutex = gcnew Mutex(false, mutexName);
		
		MemoryMappedViewStream^ fileStream = filePipe->CreateViewStream();
		unsigned char* dataptr;
		fileStream->SafeMemoryMappedViewHandle->AcquirePointer(dataptr);
		memset(dataptr, 0, golfTransmissionConsts::fileSize);
		fileStream->SafeMemoryMappedViewHandle->ReleasePointer();
		fileStream->Close();

		return true;
	}

	//Process Data Thread
	void ControlWindow::threadWorker()
	{
		try{
			if(!mutex->WaitOne(100)) return;
		
			MemoryMappedViewStream^ fileStream = filePipe->CreateViewStream();

			unsigned char* dataptr;
			fileStream->SafeMemoryMappedViewHandle->AcquirePointer(dataptr);

			golfData->logoClk = dataptr[offsetof(golfTransmission, logoClk)];
			dataptr[offsetof(golfTransmission, logoClk)] = 0;
			golfData->imagesClk = dataptr[offsetof(golfTransmission, imagesClk)];
			dataptr[offsetof(golfTransmission, imagesClk)] = 0;

			if(golfData->optionsClk)
			{
				golfData->optionsClk = 0;
				dataptr[offsetof(golfTransmission, optionsClk)] = 1;
				memcpy(dataptr+golfTransmissionConsts::optionsOffset, (char*)golfData+golfTransmissionConsts::optionsOffset, golfTransmissionConsts::optionsSize);
			}
			if(golfData->controlsClk)
			{
				golfData->controlsClk = 0;
				dataptr[offsetof(golfTransmission, controlsClk)] = 1;
				memcpy(dataptr+golfTransmissionConsts::controlsOffset, (char*)golfData+golfTransmissionConsts::controlsOffset, golfTransmissionConsts::controlsSize);
			}
			if(golfData->logoClk)
			{
				if(golfData->logoClk - 1>0)
					statisticsForm->addBall(golfData->logoClk - 1);
				statisticsForm->save();
				golfData->logoClk = 0;
			}
			
			if(golfData->imagesClk)
			{
				golfData->imagesClk = 0;

				memcpy((char*)golfData+golfTransmissionConsts::imgsizeoffset, dataptr+golfTransmissionConsts::imgsizeoffset, golfTransmissionConsts::imgsizesize);

				if(golfData->render_img1)
				{
					LoadImage(dataptr, pictureBox1, golfTransmissionConsts::imgsOffset1, golfData->img_width1, golfData->img_height1);
				}
				if(golfData->render_img2)
				{
					LoadImage(dataptr, pictureBox2, golfTransmissionConsts::imgsOffset2, golfData->img_width2, golfData->img_height2);
				}
				if(golfData->render_img3)
				{
					LoadImage(dataptr, pictureBox3, golfTransmissionConsts::imgsOffset3, golfData->img_width3, golfData->img_height3);
				}
				if(golfData->render_img4)
				{
					LoadImage(dataptr, newballForm->BallPicture, golfTransmissionConsts::imgsOffset4, golfData->img_width4, golfData->img_height4);
				}
			}

			fileStream->SafeMemoryMappedViewHandle->ReleasePointer();
			fileStream->Close();
			mutex->ReleaseMutex();
		}
		catch(Exception^ e)
		{
			MessageBox::Show(this, e->Message); 
			mutex->ReleaseMutex(); 
			return;
		}
	}

	//Process halted
	System::Void ControlWindow::sorterStopped(System::Object^  sender, System::EventArgs^  e) {
		this->statusLabel->ForeColor = System::Drawing::Color::Red;
		this->statusLabel->Text = L"Stopped";
		this->runButton->Text = L"Run";
	}

	//
	System::Void ControlWindow::SetHDStream(bool should)
	{
		golfData->controlsClk = 1;
		golfData->render_img4 = should;
	}

	//Buttons
	System::Void ControlWindow::runButtonPress(System::Object^  sender, System::EventArgs^  e) {
		bool running = false;
		try
		{
			running = !this->golfSorterProcess->HasExited;
		}
		catch(Exception^)
		{}

		if(running)
		{
			//Stop the process
			this->golfSorterProcess->Kill();
			this->runButton->Text = L"Run";
		}
		else
		{
			//Start the process
			try
			{
				golfData->controlsClk = 1;
				golfData->optionsClk = 1;
				this->golfSorterProcess->StartInfo->Arguments = filePipeName + L" " + mutexName;
				this->golfSorterProcess->Start();
				this->statusLabel->ForeColor = System::Drawing::Color::Green;
				this->statusLabel->Text = L"Running";
				this->runButton->Text = L"Stop";
			}
			catch(Exception^ e)
			{
				MessageBox::Show(this, e->Message);
			}
		}
	}
	System::Void ControlWindow::optionsButtonPress(System::Object^  sender, System::EventArgs^  e) {
		if(!optionsForm->Visible)
			optionsForm->Show(this);
	}
	System::Void ControlWindow::statisticButtonPress(System::Object^  sender, System::EventArgs^  e) {
		if(!statisticsForm->Visible)
		{
			statisticsForm->Show(this);
		}
	}
	System::Void ControlWindow::newballPress(System::Object^  sender, System::EventArgs^  e) {
		if(!newballForm->Visible)
		{
			SetHDStream(true);
			newballForm->Show(this);
		}
	}

	//Check Boxes here
	System::Void ControlWindow::showCam1Click(System::Object^  sender, System::EventArgs^  e) {
		if(this->showCam1->Checked)
		{
			golfData->render_img1 = 1;
		}
		else
		{
			golfData->render_img1 = 0;
			pictureBox1->Image = gcnew Bitmap(1,1);
		}
		golfData->controlsClk = 1;
	}
	System::Void ControlWindow::showCam2Click(System::Object^  sender, System::EventArgs^  e) {
		if(this->showCam2->Checked)
		{
			golfData->render_img2 = 1;
		}
		else
		{
			golfData->render_img2 = 0;
			pictureBox2->Image = gcnew Bitmap(1,1);
		}
		golfData->controlsClk = 1;
	}
	System::Void ControlWindow::showCam3Click(System::Object^  sender, System::EventArgs^  e) {
		if(this->showCam3->Checked)
		{
			golfData->render_img3 = 1;
		}
		else
		{
			golfData->render_img3 = 0;
			pictureBox3->Image = gcnew Bitmap(1,1);
		}
		golfData->controlsClk = 1;
	}
	System::Void ControlWindow::showlogosPress(System::Object^  sender, System::EventArgs^  e) {
		if(this->showLogos->Checked)
		{
			golfData->render_data = 1;
		}
		else
		{
			golfData->render_data = 0;
		}
		golfData->controlsClk = 1;
	}
	System::Void ControlWindow::ChangeMask(System::Object^  sender, System::EventArgs^  e) {
		if(radioButton1->Checked)
			golfData->render_mask = 0;
		else if(radioButton2->Checked)
			golfData->render_mask = 1;
		else if(radioButton3->Checked)
			golfData->render_mask = 2;
		golfData->controlsClk = 1;
	}
	System::Void ControlWindow::sortBrands(System::Object^  sender, System::EventArgs^  e) {
		if(this->sortBrandsCB->Checked)
		{
			golfData->sort_brands = 1;
		}
		else
		{
			golfData->sort_brands = 0;
		}
		golfData->controlsClk = 1;
	}
	System::Void ControlWindow::sortModels(System::Object^  sender, System::EventArgs^  e) {
		if(this->sortModelsCB->Checked)
		{
			golfData->sort_models = 1;
		}
		else
		{
			golfData->sort_models = 0;
		}
		golfData->controlsClk = 1;
	}
}