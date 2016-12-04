#pragma once

namespace ICC_interface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Net::Mail;
	using namespace System::IO;
	using namespace System::Threading;

	/// <summary>
	/// Summary for NewBall
	/// </summary>
	public ref class SnapshotWindow : public System::Windows::Forms::Form
	{
	public:
		SnapshotWindow(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SnapshotWindow()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::Button^  button1;
	protected: 

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
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(SnapshotWindow::typeid));
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(13, 13);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(2048, 500);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::AutoSize;
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Left));
			this->button1->Location = System::Drawing::Point(13, 519);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Send Image";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &SnapshotWindow::SendEmail);
			// 
			// SnapshotWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1924, 554);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"SnapshotWindow";
			this->Text = L"NewBall";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &SnapshotWindow::cancelClose);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void cancelClose(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);

	public: property PictureBox^ BallPicture{
			  PictureBox^ get() {
				 return pictureBox1;
			  }    
		   }
	private: Thread^ SendThread;
	private: System::Void SendEmail(System::Object^  sender, System::EventArgs^  e) {
				 if(button1->Enabled && pictureBox1->Image)
				 {
					 button1->Enabled = false;
					 SendThread = gcnew Thread(gcnew ThreadStart(this, &SnapshotWindow::FSendThread));
					 SendThread->Start();
					 button1->Text = L"Sending....";
				 }
			 }
	private: delegate void DoneCallback();
	private: delegate void SaveCallback(MemoryStream^ stream);
	private: delegate void ErrorCallback(String^ error);
	private: void DoneSending()
			 {
				 button1->Enabled = true;
				 button1->Text = L"Send Image";
			 }
	private: void SaveImage(MemoryStream^ stream)
			 {
				 pictureBox1->Image->Save(stream,Imaging::ImageFormat::Png);
			 }
	private: void Error(String^ e)
			 {
				 MessageBox::Show(this, e);
			 }
	private: void FSendThread();
	};
}
