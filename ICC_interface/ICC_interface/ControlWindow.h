#pragma once

namespace ICC_interface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO::MemoryMappedFiles;
	using namespace System::Threading;

	/// <summary>
	/// Summary for ControlWindow
	/// </summary>
	public ref class ControlWindow : public System::Windows::Forms::Form
	{
	public:
		MemoryMappedFile^ filePipe;
		Mutex^ mutex;

		String^ filePipeName;
		String^ mutexName;

		golfTransmission* golfData;
		
		StatisticsWindow^ statisticsForm;
		OptionsWindow^ optionsForm;
		SnapshotWindow^ newballForm;
	private: System::Windows::Forms::TableLayoutPanel^  tableLayoutPanel1;
	public: 
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::PictureBox^  pictureBox3;
	private: System::Windows::Forms::CheckBox^  showCam3;

	private: System::Windows::Forms::CheckBox^  sortModelsCB;

	private: System::Windows::Forms::CheckBox^  sortBrandsCB;
	private: System::Windows::Forms::RadioButton^  radioButton3;
	private: System::Windows::Forms::RadioButton^  radioButton2;
	private: System::Windows::Forms::RadioButton^  radioButton1;

		Thread^ worker;

	public:
		delegate void threadWorkerCallback();
		void threadWorker();
		void threadInvoker()
		{
			threadWorkerCallback^ del = gcnew threadWorkerCallback(this, &ControlWindow::threadWorker);
			while(true)
			{
				this->Invoke(del);
				Thread::Sleep(50); //20hz
			}
		}
		
		ControlWindow();
		

	protected:
		~ControlWindow()
		{
			if (components)
			{
				delete components;
			}
			delete golfData;
		}

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::Label^  statusLabel;
	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Button^  runButton;
	private: System::Windows::Forms::Button^  newballButton;
	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  optionsButton;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;
	private: System::Windows::Forms::CheckBox^  showLogos;

	private: System::Windows::Forms::CheckBox^  showCam2;
	private: System::Windows::Forms::CheckBox^  showCam1;
	private: System::Diagnostics::Process^  golfSorterProcess;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(ControlWindow::typeid));
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->statusLabel = (gcnew System::Windows::Forms::Label());
			this->runButton = (gcnew System::Windows::Forms::Button());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->newballButton = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->optionsButton = (gcnew System::Windows::Forms::Button());
			this->showLogos = (gcnew System::Windows::Forms::CheckBox());
			this->showCam2 = (gcnew System::Windows::Forms::CheckBox());
			this->showCam1 = (gcnew System::Windows::Forms::CheckBox());
			this->golfSorterProcess = (gcnew System::Diagnostics::Process());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->tableLayoutPanel1 = (gcnew System::Windows::Forms::TableLayoutPanel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->radioButton3 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton2 = (gcnew System::Windows::Forms::RadioButton());
			this->radioButton1 = (gcnew System::Windows::Forms::RadioButton());
			this->sortModelsCB = (gcnew System::Windows::Forms::CheckBox());
			this->sortBrandsCB = (gcnew System::Windows::Forms::CheckBox());
			this->showCam3 = (gcnew System::Windows::Forms::CheckBox());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->panel1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->BeginInit();
			this->tableLayoutPanel1->SuspendLayout();
			this->panel2->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(5, 29);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(40, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Status:";
			this->label1->Click += gcnew System::EventHandler(this, &ControlWindow::label1_Click);
			// 
			// statusLabel
			// 
			this->statusLabel->AutoSize = true;
			this->statusLabel->ForeColor = System::Drawing::Color::Red;
			this->statusLabel->Location = System::Drawing::Point(42, 29);
			this->statusLabel->Name = L"statusLabel";
			this->statusLabel->Size = System::Drawing::Size(47, 13);
			this->statusLabel->TabIndex = 2;
			this->statusLabel->Text = L"Stopped";
			// 
			// runButton
			// 
			this->runButton->Location = System::Drawing::Point(6, 3);
			this->runButton->Name = L"runButton";
			this->runButton->Size = System::Drawing::Size(75, 23);
			this->runButton->TabIndex = 3;
			this->runButton->Text = L"Run";
			this->runButton->UseVisualStyleBackColor = true;
			this->runButton->Click += gcnew System::EventHandler(this, &ControlWindow::runButtonPress);
			// 
			// pictureBox1
			// 
			this->pictureBox1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox1->Location = System::Drawing::Point(3, 93);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(460, 1);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox1->TabIndex = 4;
			this->pictureBox1->TabStop = false;
			// 
			// panel1
			// 
			this->panel1->Controls->Add(this->newballButton);
			this->panel1->Controls->Add(this->button2);
			this->panel1->Controls->Add(this->optionsButton);
			this->panel1->Controls->Add(this->runButton);
			this->panel1->Controls->Add(this->label1);
			this->panel1->Controls->Add(this->statusLabel);
			this->panel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel1->Location = System::Drawing::Point(3, 114);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(460, 183);
			this->panel1->TabIndex = 5;
			// 
			// newballButton
			// 
			this->newballButton->Location = System::Drawing::Point(253, 4);
			this->newballButton->Name = L"newballButton";
			this->newballButton->Size = System::Drawing::Size(75, 23);
			this->newballButton->TabIndex = 6;
			this->newballButton->Text = L"New Ball";
			this->newballButton->UseVisualStyleBackColor = true;
			this->newballButton->Click += gcnew System::EventHandler(this, &ControlWindow::newballPress);
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(171, 4);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 5;
			this->button2->Text = L"Statistics";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &ControlWindow::statisticButtonPress);
			// 
			// optionsButton
			// 
			this->optionsButton->Location = System::Drawing::Point(89, 3);
			this->optionsButton->Name = L"optionsButton";
			this->optionsButton->Size = System::Drawing::Size(75, 23);
			this->optionsButton->TabIndex = 4;
			this->optionsButton->Text = L"Options";
			this->optionsButton->UseVisualStyleBackColor = true;
			this->optionsButton->Click += gcnew System::EventHandler(this, &ControlWindow::optionsButtonPress);
			// 
			// showLogos
			// 
			this->showLogos->AutoSize = true;
			this->showLogos->Location = System::Drawing::Point(280, 3);
			this->showLogos->Name = L"showLogos";
			this->showLogos->Size = System::Drawing::Size(85, 17);
			this->showLogos->TabIndex = 3;
			this->showLogos->Text = L"Show Logos";
			this->showLogos->UseVisualStyleBackColor = true;
			this->showLogos->Click += gcnew System::EventHandler(this, &ControlWindow::showlogosPress);
			// 
			// showCam2
			// 
			this->showCam2->AutoSize = true;
			this->showCam2->Location = System::Drawing::Point(95, 3);
			this->showCam2->Name = L"showCam2";
			this->showCam2->Size = System::Drawing::Size(86, 17);
			this->showCam2->TabIndex = 2;
			this->showCam2->Text = L"Show Cam 2";
			this->showCam2->UseVisualStyleBackColor = true;
			this->showCam2->Click += gcnew System::EventHandler(this, &ControlWindow::showCam2Click);
			// 
			// showCam1
			// 
			this->showCam1->AutoSize = true;
			this->showCam1->Location = System::Drawing::Point(6, 3);
			this->showCam1->Name = L"showCam1";
			this->showCam1->Size = System::Drawing::Size(83, 17);
			this->showCam1->TabIndex = 1;
			this->showCam1->Text = L"Show Cam1";
			this->showCam1->UseVisualStyleBackColor = true;
			this->showCam1->Click += gcnew System::EventHandler(this, &ControlWindow::showCam1Click);
			// 
			// golfSorterProcess
			// 
			this->golfSorterProcess->EnableRaisingEvents = true;
			this->golfSorterProcess->StartInfo->Domain = L"";
#ifdef _DEBUG
			this->golfSorterProcess->StartInfo->FileName = L"..\\..\\ICC_Parameters\\x64\\Debug\\ICC_Golf_Balls_Project.exe";
#else
			this->golfSorterProcess->StartInfo->FileName = L"..\\..\\ICC_Golf_Balls_Project\\x64\\Release\\ICC_Golf_Balls_Project.exe";
#endif
			this->golfSorterProcess->StartInfo->LoadUserProfile = false;
			this->golfSorterProcess->StartInfo->Password = nullptr;
			this->golfSorterProcess->StartInfo->StandardErrorEncoding = nullptr;
			this->golfSorterProcess->StartInfo->StandardOutputEncoding = nullptr;
			this->golfSorterProcess->StartInfo->UserName = L"";
			this->golfSorterProcess->StartInfo->UseShellExecute = false;
			this->golfSorterProcess->SynchronizingObject = this;
			this->golfSorterProcess->Exited += gcnew System::EventHandler(this, &ControlWindow::sorterStopped);
			// 
			// pictureBox2
			// 
			this->pictureBox2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox2->Location = System::Drawing::Point(3, 100);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(460, 1);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox2->TabIndex = 6;
			this->pictureBox2->TabStop = false;
			// 
			// tableLayoutPanel1
			// 
			this->tableLayoutPanel1->ColumnCount = 1;
			this->tableLayoutPanel1->ColumnStyles->Add((gcnew System::Windows::Forms::ColumnStyle(System::Windows::Forms::SizeType::Percent, 
				100)));
			this->tableLayoutPanel1->Controls->Add(this->panel1, 0, 4);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox2, 0, 2);
			this->tableLayoutPanel1->Controls->Add(this->panel2, 0, 0);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox1, 0, 1);
			this->tableLayoutPanel1->Controls->Add(this->pictureBox3, 0, 3);
			this->tableLayoutPanel1->Dock = System::Windows::Forms::DockStyle::Fill;
			this->tableLayoutPanel1->Location = System::Drawing::Point(0, 0);
			this->tableLayoutPanel1->Name = L"tableLayoutPanel1";
			this->tableLayoutPanel1->RowCount = 5;
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 90)));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle()));
			this->tableLayoutPanel1->RowStyles->Add((gcnew System::Windows::Forms::RowStyle(System::Windows::Forms::SizeType::Absolute, 50)));
			this->tableLayoutPanel1->Size = System::Drawing::Size(466, 300);
			this->tableLayoutPanel1->TabIndex = 7;
			// 
			// panel2
			// 
			this->panel2->Controls->Add(this->radioButton3);
			this->panel2->Controls->Add(this->radioButton2);
			this->panel2->Controls->Add(this->radioButton1);
			this->panel2->Controls->Add(this->sortModelsCB);
			this->panel2->Controls->Add(this->sortBrandsCB);
			this->panel2->Controls->Add(this->showCam3);
			this->panel2->Controls->Add(this->showLogos);
			this->panel2->Controls->Add(this->showCam1);
			this->panel2->Controls->Add(this->showCam2);
			this->panel2->Dock = System::Windows::Forms::DockStyle::Fill;
			this->panel2->Location = System::Drawing::Point(3, 3);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(460, 84);
			this->panel2->TabIndex = 6;
			// 
			// radioButton3
			// 
			this->radioButton3->AutoSize = true;
			this->radioButton3->Location = System::Drawing::Point(170, 26);
			this->radioButton3->Name = L"radioButton3";
			this->radioButton3->Size = System::Drawing::Size(98, 17);
			this->radioButton3->TabIndex = 10;
			this->radioButton3->Text = L"Ball Logo Mask";
			this->radioButton3->UseVisualStyleBackColor = true;
			this->radioButton3->Click += gcnew System::EventHandler(this, &ControlWindow::ChangeMask);
			// 
			// radioButton2
			// 
			this->radioButton2->AutoSize = true;
			this->radioButton2->Location = System::Drawing::Point(70, 26);
			this->radioButton2->Name = L"radioButton2";
			this->radioButton2->Size = System::Drawing::Size(94, 17);
			this->radioButton2->TabIndex = 9;
			this->radioButton2->Text = L"Blue Bar Mask";
			this->radioButton2->UseVisualStyleBackColor = true;
			this->radioButton2->Click += gcnew System::EventHandler(this, &ControlWindow::ChangeMask);
			// 
			// radioButton1
			// 
			this->radioButton1->AutoSize = true;
			this->radioButton1->Checked = true;
			this->radioButton1->Location = System::Drawing::Point(6, 26);
			this->radioButton1->Name = L"radioButton1";
			this->radioButton1->Size = System::Drawing::Size(58, 17);
			this->radioButton1->TabIndex = 8;
			this->radioButton1->TabStop = true;
			this->radioButton1->Text = L"Normal";
			this->radioButton1->UseVisualStyleBackColor = true;
			this->radioButton1->Click += gcnew System::EventHandler(this, &ControlWindow::ChangeMask);
			// 
			// sortModelsCB
			// 
			this->sortModelsCB->AutoSize = true;
			this->sortModelsCB->Location = System::Drawing::Point(95, 49);
			this->sortModelsCB->Name = L"sortModelsCB";
			this->sortModelsCB->Size = System::Drawing::Size(82, 17);
			this->sortModelsCB->TabIndex = 7;
			this->sortModelsCB->Text = L"Sort Models";
			this->sortModelsCB->UseVisualStyleBackColor = true;
			this->sortModelsCB->Click += gcnew System::EventHandler(this, &ControlWindow::sortModels);
			// 
			// sortBrandsCB
			// 
			this->sortBrandsCB->AutoSize = true;
			this->sortBrandsCB->Location = System::Drawing::Point(6, 49);
			this->sortBrandsCB->Name = L"sortBrandsCB";
			this->sortBrandsCB->Size = System::Drawing::Size(81, 17);
			this->sortBrandsCB->TabIndex = 6;
			this->sortBrandsCB->Text = L"Sort Brands";
			this->sortBrandsCB->UseVisualStyleBackColor = true;
			this->sortBrandsCB->Click += gcnew System::EventHandler(this, &ControlWindow::sortBrands);
			// 
			// showCam3
			// 
			this->showCam3->AutoSize = true;
			this->showCam3->Location = System::Drawing::Point(188, 3);
			this->showCam3->Name = L"showCam3";
			this->showCam3->Size = System::Drawing::Size(86, 17);
			this->showCam3->TabIndex = 4;
			this->showCam3->Text = L"Show Cam 3";
			this->showCam3->UseVisualStyleBackColor = true;
			this->showCam3->Click += gcnew System::EventHandler(this, &ControlWindow::showCam3Click);
			// 
			// pictureBox3
			// 
			this->pictureBox3->Dock = System::Windows::Forms::DockStyle::Fill;
			this->pictureBox3->Location = System::Drawing::Point(3, 107);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(460, 1);
			this->pictureBox3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox3->TabIndex = 7;
			this->pictureBox3->TabStop = false;
			// 
			// ControlWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->AutoScroll = true;
			this->ClientSize = System::Drawing::Size(466, 300);
			this->Controls->Add(this->tableLayoutPanel1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"ControlWindow";
			this->Text = L"Golf Ball Sorting Interface";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &ControlWindow::KillProcess);
			this->Load += gcnew System::EventHandler(this, &ControlWindow::ControlWindow_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->panel1->ResumeLayout(false);
			this->panel1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox2))->EndInit();
			this->tableLayoutPanel1->ResumeLayout(false);
			this->tableLayoutPanel1->PerformLayout();
			this->panel2->ResumeLayout(false);
			this->panel2->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox3))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	
	private: System::Void sorterStopped(System::Object^  sender, System::EventArgs^  e);
	private: System::Void runButtonPress(System::Object^  sender, System::EventArgs^  e);
	private: System::Void showCam1Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void showCam2Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void showCam3Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void optionsButtonPress(System::Object^  sender, System::EventArgs^  e);
	private: System::Void statisticButtonPress(System::Object^  sender, System::EventArgs^  e);
	private: System::Void newballPress(System::Object^  sender, System::EventArgs^  e);
	private: System::Void showlogosPress(System::Object^  sender, System::EventArgs^  e);
	private: System::Void KillProcess(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
				 try{golfSorterProcess->Kill();}catch(Exception^){}
				 try{worker->Abort();}catch(Exception^){}
			 }
	public: System::Void SetHDStream(bool);
	public: bool Init();
private: System::Void statusLabel_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void label1_Click(System::Object^  sender, System::EventArgs^  e) {
		 }
private: System::Void ControlWindow_Load(System::Object^  sender, System::EventArgs^  e) {
			worker = gcnew Thread(gcnew ThreadStart(this,&ControlWindow::threadInvoker));
			worker->Start();
		 }
private: System::Void sortBrands(System::Object^  sender, System::EventArgs^  e);
private: System::Void sortModels(System::Object^  sender, System::EventArgs^  e);
private: System::Void ChangeMask(System::Object^  sender, System::EventArgs^  e);
};
}
