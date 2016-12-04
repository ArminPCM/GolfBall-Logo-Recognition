#include "stdafx.h"
#include "SnapshotWindow.h"
#include "ControlWindow.h"

namespace ICC_interface
{
	System::Void SnapshotWindow::cancelClose(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
		if (e->CloseReason != CloseReason::UserClosing) return;
		if(this->Parent) ((ControlWindow^)this->Parent)->SetHDStream(false);
		this->pictureBox1->Image = nullptr;
		this->Hide();
		e->Cancel = true;
	}

	void SnapshotWindow::FSendThread()
	{
		MailMessage^ message = gcnew MailMessage(L"astrautarlington@gmail.com", L"astrautarlington@gmail.com", L"New Golfball Image", L"New brand");
				 
		MemoryStream^ imgStream = gcnew MemoryStream();
		SaveCallback^ savecb = gcnew SaveCallback(this, &SnapshotWindow::SaveImage);
		this->Invoke(savecb, gcnew array<Object^>{imgStream});

		imgStream->Position = 0;
		Attachment^ image = gcnew Attachment(imgStream, L"Image.png", L"image/png");

		message->Attachments->Add(image);

		SmtpClient^ email = gcnew SmtpClient(L"smtp.gmail.com");
		email->Port = 587;
		email->DeliveryMethod = SmtpDeliveryMethod::Network;
		email->EnableSsl = true;
		email->UseDefaultCredentials = false;
		email->Credentials = gcnew System::Net::NetworkCredential(L"astrautarlington",L"astra413");
		try
		{
			email->Send(message);
		}
		catch(Exception^ e)
		{
			ErrorCallback^ errorCB = gcnew ErrorCallback(this, &SnapshotWindow::Error);
			this->Invoke(errorCB, gcnew array<Object^>{e->Message});
		}

		DoneCallback^ done = gcnew DoneCallback(this, &SnapshotWindow::DoneSending);
		this->Invoke(done);
	}
}