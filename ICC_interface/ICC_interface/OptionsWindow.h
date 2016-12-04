#pragma once

namespace ICC_interface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for OptionsWindow
	/// </summary>
	public ref class OptionsWindow : public System::Windows::Forms::Form
	{
	public:
		OptionsWindow(void)
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
		~OptionsWindow()
		{
			if (components)
			{
				delete components;
			}
		}

	protected: 
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label4;


	private: System::Windows::Forms::Label^  label3;

	private: System::Windows::Forms::Label^  label1;
	private: System::Windows::Forms::TextBox^  textBox0;

	private: System::Windows::Forms::Label^  label;
	private: System::Windows::Forms::TextBox^  textBox1;



	private: System::Windows::Forms::Label^  label6;

	private: System::Windows::Forms::ColorDialog^  colorDialog1;
	private: System::Windows::Forms::Label^  label7;

	private: System::Windows::Forms::Label^  label8;

	private: System::Windows::Forms::Label^  label9;

	private: System::Windows::Forms::Label^  label10;

	private: System::Windows::Forms::Label^  label11;






	private: System::Windows::Forms::TextBox^  textBox2;
	private: System::Windows::Forms::TextBox^  textBox3;
	private: System::Windows::Forms::TextBox^  textBox4;
	private: System::Windows::Forms::TextBox^  textBox5;
	private: System::Windows::Forms::TextBox^  textBox6;
	private: System::Windows::Forms::TextBox^  textBox7;
	private: System::Windows::Forms::TextBox^  textBox8;
	private: System::Windows::Forms::TextBox^  textBox9;
	private: System::Windows::Forms::TextBox^  textBox10;


	private: System::Windows::Forms::Panel^  panel1;
	private: System::Windows::Forms::Panel^  panel2;
	private: System::Windows::Forms::Label^  label14;
	private: System::Windows::Forms::Label^  label15;

	private: System::Windows::Forms::Button^  button2;
	private: System::Windows::Forms::Button^  button3;
	private: System::Windows::Forms::TextBox^  textBox13;
	private: System::Windows::Forms::Label^  label16;
	private: System::Windows::Forms::Label^  label17;
	private: System::Windows::Forms::TextBox^  textBox14;
	private: System::Windows::Forms::TextBox^  textBox15;
	private: System::Windows::Forms::TextBox^  textBox16;
	private: System::Windows::Forms::Label^  label18;
	private: System::Windows::Forms::Label^  label19;
	private: System::Windows::Forms::TextBox^  textBox17;
	private: System::Windows::Forms::TextBox^  textBox18;
	private: System::Windows::Forms::Label^  label20;
	private: System::Windows::Forms::Label^  label21;
	private: System::Windows::Forms::TextBox^  textBox19;
	private: System::Windows::Forms::TextBox^  textBox20;
	private: System::Windows::Forms::Label^  label22;
	private: System::Windows::Forms::Label^  label23;
	private: System::Windows::Forms::Label^  label24;
	private: System::Windows::Forms::TextBox^  textBox21;
	private: System::Windows::Forms::Label^  label25;
	private: System::Windows::Forms::TextBox^  textBox22;
	private: System::Windows::Forms::TextBox^  textBox11;
	private: System::Windows::Forms::Label^  label2;
	private: System::Windows::Forms::Label^  label12;
	private: System::Windows::Forms::Label^  label13;
	private: System::Windows::Forms::Label^  label26;
	private: System::Windows::Forms::TextBox^  textBox23;
	private: System::Windows::Forms::TextBox^  textBox24;
	private: System::Windows::Forms::TextBox^  textBox25;
	private: System::ComponentModel::IContainer^  components;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(OptionsWindow::typeid));
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label3 = (gcnew System::Windows::Forms::Label());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->textBox0 = (gcnew System::Windows::Forms::TextBox());
			this->label = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->label6 = (gcnew System::Windows::Forms::Label());
			this->colorDialog1 = (gcnew System::Windows::Forms::ColorDialog());
			this->label7 = (gcnew System::Windows::Forms::Label());
			this->label8 = (gcnew System::Windows::Forms::Label());
			this->label9 = (gcnew System::Windows::Forms::Label());
			this->label10 = (gcnew System::Windows::Forms::Label());
			this->label11 = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			this->textBox3 = (gcnew System::Windows::Forms::TextBox());
			this->textBox4 = (gcnew System::Windows::Forms::TextBox());
			this->textBox5 = (gcnew System::Windows::Forms::TextBox());
			this->textBox6 = (gcnew System::Windows::Forms::TextBox());
			this->textBox7 = (gcnew System::Windows::Forms::TextBox());
			this->textBox8 = (gcnew System::Windows::Forms::TextBox());
			this->textBox9 = (gcnew System::Windows::Forms::TextBox());
			this->textBox10 = (gcnew System::Windows::Forms::TextBox());
			this->panel1 = (gcnew System::Windows::Forms::Panel());
			this->panel2 = (gcnew System::Windows::Forms::Panel());
			this->label14 = (gcnew System::Windows::Forms::Label());
			this->label15 = (gcnew System::Windows::Forms::Label());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->button3 = (gcnew System::Windows::Forms::Button());
			this->textBox13 = (gcnew System::Windows::Forms::TextBox());
			this->label16 = (gcnew System::Windows::Forms::Label());
			this->label17 = (gcnew System::Windows::Forms::Label());
			this->textBox14 = (gcnew System::Windows::Forms::TextBox());
			this->textBox15 = (gcnew System::Windows::Forms::TextBox());
			this->textBox16 = (gcnew System::Windows::Forms::TextBox());
			this->label18 = (gcnew System::Windows::Forms::Label());
			this->label19 = (gcnew System::Windows::Forms::Label());
			this->textBox17 = (gcnew System::Windows::Forms::TextBox());
			this->textBox18 = (gcnew System::Windows::Forms::TextBox());
			this->label20 = (gcnew System::Windows::Forms::Label());
			this->label21 = (gcnew System::Windows::Forms::Label());
			this->textBox19 = (gcnew System::Windows::Forms::TextBox());
			this->textBox20 = (gcnew System::Windows::Forms::TextBox());
			this->label22 = (gcnew System::Windows::Forms::Label());
			this->label23 = (gcnew System::Windows::Forms::Label());
			this->label24 = (gcnew System::Windows::Forms::Label());
			this->textBox21 = (gcnew System::Windows::Forms::TextBox());
			this->label25 = (gcnew System::Windows::Forms::Label());
			this->textBox22 = (gcnew System::Windows::Forms::TextBox());
			this->textBox11 = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->label12 = (gcnew System::Windows::Forms::Label());
			this->label13 = (gcnew System::Windows::Forms::Label());
			this->label26 = (gcnew System::Windows::Forms::Label());
			this->textBox23 = (gcnew System::Windows::Forms::TextBox());
			this->textBox24 = (gcnew System::Windows::Forms::TextBox());
			this->textBox25 = (gcnew System::Windows::Forms::TextBox());
			this->SuspendLayout();
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(269, 118);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(69, 13);
			this->label5->TabIndex = 15;
			this->label5->Text = L"Cam 2 ROI Y";
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(269, 92);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(90, 13);
			this->label4->TabIndex = 13;
			this->label4->Text = L"Cam1 ROI Height";
			// 
			// label3
			// 
			this->label3->AutoSize = true;
			this->label3->Location = System::Drawing::Point(269, 66);
			this->label3->Name = L"label3";
			this->label3->Size = System::Drawing::Size(66, 13);
			this->label3->TabIndex = 11;
			this->label3->Text = L"Cam1 ROI Y";
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(11, 66);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(72, 13);
			this->label1->TabIndex = 16;
			this->label1->Text = L"Sift Threshold";
			// 
			// textBox0
			// 
			this->textBox0->Location = System::Drawing::Point(144, 63);
			this->textBox0->Name = L"textBox0";
			this->textBox0->Size = System::Drawing::Size(100, 20);
			this->textBox0->TabIndex = 17;
			this->textBox0->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// label
			// 
			this->label->AutoSize = true;
			this->label->Location = System::Drawing::Point(11, 92);
			this->label->Name = L"label";
			this->label->Size = System::Drawing::Size(89, 13);
			this->label->TabIndex = 18;
			this->label->Text = L"Max Components";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(144, 89);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(100, 20);
			this->textBox1->TabIndex = 19;
			this->textBox1->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// label6
			// 
			this->label6->AutoSize = true;
			this->label6->Location = System::Drawing::Point(269, 144);
			this->label6->Name = L"label6";
			this->label6->Size = System::Drawing::Size(93, 13);
			this->label6->TabIndex = 23;
			this->label6->Text = L"Cam 2 ROI Height";
			// 
			// label7
			// 
			this->label7->AutoSize = true;
			this->label7->Location = System::Drawing::Point(11, 118);
			this->label7->Name = L"label7";
			this->label7->Size = System::Drawing::Size(47, 13);
			this->label7->TabIndex = 25;
			this->label7->Text = L"Ball Size";
			// 
			// label8
			// 
			this->label8->AutoSize = true;
			this->label8->Location = System::Drawing::Point(11, 144);
			this->label8->Name = L"label8";
			this->label8->Size = System::Drawing::Size(93, 13);
			this->label8->TabIndex = 27;
			this->label8->Text = L"Search Window X";
			// 
			// label9
			// 
			this->label9->AutoSize = true;
			this->label9->Location = System::Drawing::Point(11, 170);
			this->label9->Name = L"label9";
			this->label9->Size = System::Drawing::Size(93, 13);
			this->label9->TabIndex = 29;
			this->label9->Text = L"Search Window Y";
			// 
			// label10
			// 
			this->label10->AutoSize = true;
			this->label10->Location = System::Drawing::Point(11, 196);
			this->label10->Name = L"label10";
			this->label10->Size = System::Drawing::Size(114, 13);
			this->label10->TabIndex = 31;
			this->label10->Text = L"Search Window Width";
			// 
			// label11
			// 
			this->label11->AutoSize = true;
			this->label11->Location = System::Drawing::Point(11, 222);
			this->label11->Name = L"label11";
			this->label11->Size = System::Drawing::Size(117, 13);
			this->label11->TabIndex = 33;
			this->label11->Text = L"Search Window Height";
			// 
			// textBox2
			// 
			this->textBox2->Location = System::Drawing::Point(402, 63);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(100, 20);
			this->textBox2->TabIndex = 38;
			this->textBox2->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox3
			// 
			this->textBox3->Location = System::Drawing::Point(402, 89);
			this->textBox3->Name = L"textBox3";
			this->textBox3->Size = System::Drawing::Size(100, 20);
			this->textBox3->TabIndex = 39;
			this->textBox3->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox4
			// 
			this->textBox4->Location = System::Drawing::Point(402, 115);
			this->textBox4->Name = L"textBox4";
			this->textBox4->Size = System::Drawing::Size(100, 20);
			this->textBox4->TabIndex = 40;
			this->textBox4->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox5
			// 
			this->textBox5->Location = System::Drawing::Point(402, 141);
			this->textBox5->Name = L"textBox5";
			this->textBox5->Size = System::Drawing::Size(100, 20);
			this->textBox5->TabIndex = 41;
			this->textBox5->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox6
			// 
			this->textBox6->Location = System::Drawing::Point(144, 115);
			this->textBox6->Name = L"textBox6";
			this->textBox6->Size = System::Drawing::Size(100, 20);
			this->textBox6->TabIndex = 42;
			this->textBox6->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox7
			// 
			this->textBox7->Location = System::Drawing::Point(144, 141);
			this->textBox7->Name = L"textBox7";
			this->textBox7->Size = System::Drawing::Size(100, 20);
			this->textBox7->TabIndex = 43;
			this->textBox7->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox8
			// 
			this->textBox8->Location = System::Drawing::Point(144, 167);
			this->textBox8->Name = L"textBox8";
			this->textBox8->Size = System::Drawing::Size(100, 20);
			this->textBox8->TabIndex = 44;
			this->textBox8->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox9
			// 
			this->textBox9->Location = System::Drawing::Point(144, 193);
			this->textBox9->Name = L"textBox9";
			this->textBox9->Size = System::Drawing::Size(100, 20);
			this->textBox9->TabIndex = 45;
			this->textBox9->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox10
			// 
			this->textBox10->Location = System::Drawing::Point(144, 219);
			this->textBox10->Name = L"textBox10";
			this->textBox10->Size = System::Drawing::Size(100, 20);
			this->textBox10->TabIndex = 46;
			this->textBox10->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// panel1
			// 
			this->panel1->Location = System::Drawing::Point(144, 12);
			this->panel1->Name = L"panel1";
			this->panel1->Size = System::Drawing::Size(32, 32);
			this->panel1->TabIndex = 49;
			this->panel1->Click += gcnew System::EventHandler(this, &OptionsWindow::setColorLow);
			// 
			// panel2
			// 
			this->panel2->Location = System::Drawing::Point(212, 12);
			this->panel2->Name = L"panel2";
			this->panel2->Size = System::Drawing::Size(32, 32);
			this->panel2->TabIndex = 50;
			this->panel2->Click += gcnew System::EventHandler(this, &OptionsWindow::setColorHigh);
			// 
			// label14
			// 
			this->label14->AutoSize = true;
			this->label14->Location = System::Drawing::Point(250, 12);
			this->label14->Name = L"label14";
			this->label14->Size = System::Drawing::Size(75, 13);
			this->label14->TabIndex = 51;
			this->label14->Text = L"Bar Color High";
			// 
			// label15
			// 
			this->label15->AutoSize = true;
			this->label15->Location = System::Drawing::Point(65, 12);
			this->label15->Name = L"label15";
			this->label15->Size = System::Drawing::Size(73, 13);
			this->label15->TabIndex = 52;
			this->label15->Text = L"Bar Color Low";
			// 
			// button2
			// 
			this->button2->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button2->Enabled = false;
			this->button2->Location = System::Drawing::Point(430, 580);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(75, 23);
			this->button2->TabIndex = 54;
			this->button2->Text = L"Apply";
			this->button2->UseVisualStyleBackColor = true;
			this->button2->Click += gcnew System::EventHandler(this, &OptionsWindow::applyOptions);
			// 
			// button3
			// 
			this->button3->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button3->Location = System::Drawing::Point(349, 580);
			this->button3->Name = L"button3";
			this->button3->Size = System::Drawing::Size(75, 23);
			this->button3->TabIndex = 55;
			this->button3->Text = L"Close";
			this->button3->UseVisualStyleBackColor = true;
			this->button3->Click += gcnew System::EventHandler(this, &OptionsWindow::cancelOptions);
			// 
			// textBox13
			// 
			this->textBox13->Location = System::Drawing::Point(402, 271);
			this->textBox13->Name = L"textBox13";
			this->textBox13->Size = System::Drawing::Size(100, 20);
			this->textBox13->TabIndex = 56;
			this->textBox13->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// label16
			// 
			this->label16->AutoSize = true;
			this->label16->Location = System::Drawing::Point(269, 274);
			this->label16->Name = L"label16";
			this->label16->Size = System::Drawing::Size(75, 13);
			this->label16->TabIndex = 57;
			this->label16->Text = L"Cam 2 OffsetX";
			// 
			// label17
			// 
			this->label17->AutoSize = true;
			this->label17->Location = System::Drawing::Point(269, 300);
			this->label17->Name = L"label17";
			this->label17->Size = System::Drawing::Size(75, 13);
			this->label17->TabIndex = 58;
			this->label17->Text = L"Cam 2 OffsetY";
			// 
			// textBox14
			// 
			this->textBox14->Location = System::Drawing::Point(402, 297);
			this->textBox14->Name = L"textBox14";
			this->textBox14->Size = System::Drawing::Size(100, 20);
			this->textBox14->TabIndex = 59;
			this->textBox14->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox15
			// 
			this->textBox15->Location = System::Drawing::Point(402, 323);
			this->textBox15->Name = L"textBox15";
			this->textBox15->Size = System::Drawing::Size(100, 20);
			this->textBox15->TabIndex = 60;
			this->textBox15->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox16
			// 
			this->textBox16->Location = System::Drawing::Point(402, 349);
			this->textBox16->Name = L"textBox16";
			this->textBox16->Size = System::Drawing::Size(100, 20);
			this->textBox16->TabIndex = 61;
			this->textBox16->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// label18
			// 
			this->label18->AutoSize = true;
			this->label18->Location = System::Drawing::Point(269, 326);
			this->label18->Name = L"label18";
			this->label18->Size = System::Drawing::Size(75, 13);
			this->label18->TabIndex = 62;
			this->label18->Text = L"Cam 3 OffsetX";
			// 
			// label19
			// 
			this->label19->AutoSize = true;
			this->label19->Location = System::Drawing::Point(269, 352);
			this->label19->Name = L"label19";
			this->label19->Size = System::Drawing::Size(75, 13);
			this->label19->TabIndex = 63;
			this->label19->Text = L"Cam 3 OffsetY";
			// 
			// textBox17
			// 
			this->textBox17->Location = System::Drawing::Point(402, 167);
			this->textBox17->Name = L"textBox17";
			this->textBox17->Size = System::Drawing::Size(100, 20);
			this->textBox17->TabIndex = 64;
			this->textBox17->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox18
			// 
			this->textBox18->Location = System::Drawing::Point(402, 193);
			this->textBox18->Name = L"textBox18";
			this->textBox18->Size = System::Drawing::Size(100, 20);
			this->textBox18->TabIndex = 65;
			this->textBox18->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// label20
			// 
			this->label20->AutoSize = true;
			this->label20->Location = System::Drawing::Point(269, 170);
			this->label20->Name = L"label20";
			this->label20->Size = System::Drawing::Size(69, 13);
			this->label20->TabIndex = 66;
			this->label20->Text = L"Cam 3 ROI Y";
			// 
			// label21
			// 
			this->label21->AutoSize = true;
			this->label21->Location = System::Drawing::Point(269, 196);
			this->label21->Name = L"label21";
			this->label21->Size = System::Drawing::Size(93, 13);
			this->label21->TabIndex = 67;
			this->label21->Text = L"Cam 3 ROI Height";
			// 
			// textBox19
			// 
			this->textBox19->Location = System::Drawing::Point(402, 375);
			this->textBox19->Name = L"textBox19";
			this->textBox19->Size = System::Drawing::Size(100, 20);
			this->textBox19->TabIndex = 68;
			this->textBox19->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox20
			// 
			this->textBox20->Location = System::Drawing::Point(402, 401);
			this->textBox20->Name = L"textBox20";
			this->textBox20->Size = System::Drawing::Size(100, 20);
			this->textBox20->TabIndex = 69;
			this->textBox20->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// label22
			// 
			this->label22->AutoSize = true;
			this->label22->Location = System::Drawing::Point(269, 378);
			this->label22->Name = L"label22";
			this->label22->Size = System::Drawing::Size(67, 13);
			this->label22->TabIndex = 70;
			this->label22->Text = L"Cam 2 Scale";
			// 
			// label23
			// 
			this->label23->AutoSize = true;
			this->label23->Location = System::Drawing::Point(269, 404);
			this->label23->Name = L"label23";
			this->label23->Size = System::Drawing::Size(67, 13);
			this->label23->TabIndex = 71;
			this->label23->Text = L"Cam 3 Scale";
			// 
			// label24
			// 
			this->label24->AutoSize = true;
			this->label24->Location = System::Drawing::Point(269, 222);
			this->label24->Name = L"label24";
			this->label24->Size = System::Drawing::Size(75, 13);
			this->label24->TabIndex = 72;
			this->label24->Text = L"Cam 1 OffsetX";
			// 
			// textBox21
			// 
			this->textBox21->Location = System::Drawing::Point(402, 219);
			this->textBox21->Name = L"textBox21";
			this->textBox21->Size = System::Drawing::Size(100, 20);
			this->textBox21->TabIndex = 73;
			this->textBox21->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// label25
			// 
			this->label25->AutoSize = true;
			this->label25->Location = System::Drawing::Point(269, 248);
			this->label25->Name = L"label25";
			this->label25->Size = System::Drawing::Size(75, 13);
			this->label25->TabIndex = 74;
			this->label25->Text = L"Cam 1 OffsetY";
			// 
			// textBox22
			// 
			this->textBox22->Location = System::Drawing::Point(402, 245);
			this->textBox22->Name = L"textBox22";
			this->textBox22->Size = System::Drawing::Size(100, 20);
			this->textBox22->TabIndex = 75;
			this->textBox22->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox11
			// 
			this->textBox11->Location = System::Drawing::Point(144, 245);
			this->textBox11->Name = L"textBox11";
			this->textBox11->Size = System::Drawing::Size(100, 20);
			this->textBox11->TabIndex = 76;
			this->textBox11->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(11, 248);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(83, 13);
			this->label2->TabIndex = 78;
			this->label2->Text = L"Ball Mask Scale";
			// 
			// label12
			// 
			this->label12->AutoSize = true;
			this->label12->Location = System::Drawing::Point(11, 274);
			this->label12->Name = L"label12";
			this->label12->Size = System::Drawing::Size(111, 13);
			this->label12->TabIndex = 79;
			this->label12->Text = L"Cam1 Logo Threshold";
			// 
			// label13
			// 
			this->label13->AutoSize = true;
			this->label13->Location = System::Drawing::Point(11, 300);
			this->label13->Name = L"label13";
			this->label13->Size = System::Drawing::Size(111, 13);
			this->label13->TabIndex = 80;
			this->label13->Text = L"Cam2 Logo Threshold";
			// 
			// label26
			// 
			this->label26->AutoSize = true;
			this->label26->Location = System::Drawing::Point(11, 326);
			this->label26->Name = L"label26";
			this->label26->Size = System::Drawing::Size(111, 13);
			this->label26->TabIndex = 81;
			this->label26->Text = L"Cam3 Logo Threshold";
			// 
			// textBox23
			// 
			this->textBox23->Location = System::Drawing::Point(144, 271);
			this->textBox23->Name = L"textBox23";
			this->textBox23->Size = System::Drawing::Size(100, 20);
			this->textBox23->TabIndex = 82;
			this->textBox23->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox24
			// 
			this->textBox24->Location = System::Drawing::Point(144, 297);
			this->textBox24->Name = L"textBox24";
			this->textBox24->Size = System::Drawing::Size(100, 20);
			this->textBox24->TabIndex = 83;
			this->textBox24->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// textBox25
			// 
			this->textBox25->Location = System::Drawing::Point(144, 323);
			this->textBox25->Name = L"textBox25";
			this->textBox25->Size = System::Drawing::Size(100, 20);
			this->textBox25->TabIndex = 84;
			this->textBox25->TextChanged += gcnew System::EventHandler(this, &OptionsWindow::allowApply);
			// 
			// OptionsWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(517, 615);
			this->Controls->Add(this->textBox25);
			this->Controls->Add(this->textBox24);
			this->Controls->Add(this->textBox23);
			this->Controls->Add(this->label26);
			this->Controls->Add(this->label13);
			this->Controls->Add(this->label12);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->textBox11);
			this->Controls->Add(this->textBox22);
			this->Controls->Add(this->label25);
			this->Controls->Add(this->textBox21);
			this->Controls->Add(this->label24);
			this->Controls->Add(this->label23);
			this->Controls->Add(this->label22);
			this->Controls->Add(this->textBox20);
			this->Controls->Add(this->textBox19);
			this->Controls->Add(this->label21);
			this->Controls->Add(this->label20);
			this->Controls->Add(this->textBox18);
			this->Controls->Add(this->textBox17);
			this->Controls->Add(this->label19);
			this->Controls->Add(this->label18);
			this->Controls->Add(this->textBox16);
			this->Controls->Add(this->textBox15);
			this->Controls->Add(this->textBox14);
			this->Controls->Add(this->label17);
			this->Controls->Add(this->label16);
			this->Controls->Add(this->textBox13);
			this->Controls->Add(this->button3);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->label15);
			this->Controls->Add(this->label14);
			this->Controls->Add(this->panel2);
			this->Controls->Add(this->panel1);
			this->Controls->Add(this->textBox10);
			this->Controls->Add(this->textBox9);
			this->Controls->Add(this->textBox8);
			this->Controls->Add(this->textBox7);
			this->Controls->Add(this->textBox6);
			this->Controls->Add(this->textBox5);
			this->Controls->Add(this->textBox4);
			this->Controls->Add(this->textBox3);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->label11);
			this->Controls->Add(this->label10);
			this->Controls->Add(this->label9);
			this->Controls->Add(this->label8);
			this->Controls->Add(this->label7);
			this->Controls->Add(this->label6);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->label);
			this->Controls->Add(this->textBox0);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->label3);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"OptionsWindow";
			this->Text = L"Options";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &OptionsWindow::cancelClose);
			this->VisibleChanged += gcnew System::EventHandler(this, &OptionsWindow::updateControls);
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	public: golfTransmission* golfData;
	private: System::Void cancelClose(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
				 if (e->CloseReason != CloseReason::UserClosing) return;
				 this->Hide();
				 e->Cancel = true;
			 }
	public: void OptionsLoaded() {
				panel1->BackColor = Color::FromArgb(golfData->r_low, golfData->g_low, golfData->b_low);
				panel2->BackColor = Color::FromArgb(golfData->r_high, golfData->g_high, golfData->b_high);
				textBox0->Text = Convert::ToString(golfData->sift_thresh);
				textBox1->Text = Convert::ToString(golfData->max_compenents);
				textBox2->Text = Convert::ToString(golfData->roi_y);
				textBox3->Text = Convert::ToString(golfData->roi_height);
				textBox4->Text = Convert::ToString(golfData->roi_y2);
				textBox5->Text = Convert::ToString(golfData->roi_height2);
				textBox6->Text = Convert::ToString(golfData->ballsize);
				textBox7->Text = Convert::ToString(golfData->entrance_x);
				textBox8->Text = Convert::ToString(golfData->entrance_y);
				textBox9->Text = Convert::ToString(golfData->entrance_w);
				textBox10->Text = Convert::ToString(golfData->entrance_h);
				textBox11->Text = Convert::ToString(golfData->ballmaskscale);
				textBox13->Text = Convert::ToString(golfData->cam2Offsetx);
				textBox14->Text = Convert::ToString(golfData->cam2Offsety);
				textBox15->Text = Convert::ToString(golfData->cam3Offsetx);
				textBox16->Text = Convert::ToString(golfData->cam3Offsety);
				textBox17->Text = Convert::ToString(golfData->roi_y3);
				textBox18->Text = Convert::ToString(golfData->roi_height3);
				textBox19->Text = Convert::ToString(golfData->cam2Scale);
				textBox20->Text = Convert::ToString(golfData->cam3Scale);
				textBox21->Text = Convert::ToString(golfData->cam1Offsetx);
				textBox22->Text = Convert::ToString(golfData->cam1Offsety);
				textBox23->Text = Convert::ToString((unsigned char)golfData->cam1thresh);
				textBox24->Text = Convert::ToString((unsigned char)golfData->cam2thresh);
				textBox25->Text = Convert::ToString((unsigned char)golfData->cam3thresh);
			}
	public: bool OptionsSaved() {
				bool error = false;
				golfData->r_low = panel1->BackColor.R;
				golfData->g_low = panel1->BackColor.G;
				golfData->b_low = panel1->BackColor.B;
				golfData->r_high = panel2->BackColor.R;
				golfData->g_high = panel2->BackColor.G;
				golfData->b_high = panel2->BackColor.B;

				try{golfData->sift_thresh = Convert::ToDouble(textBox0->Text);
				textBox0->BackColor = Color::White;}
				catch(Exception^){textBox0->BackColor = Color::Red; error = true;}

				try{golfData->max_compenents = Convert::ToInt32(textBox1->Text);
				textBox1->BackColor = Color::White;}
				catch(Exception^){textBox1->BackColor = Color::Red; error = true;}

				try{golfData->roi_y = Convert::ToInt32(textBox2->Text);
				textBox2->BackColor = Color::White;}
				catch(Exception^){textBox2->BackColor = Color::Red; error = true;}

				try{golfData->roi_height = Convert::ToInt32(textBox3->Text);
				textBox3->BackColor = Color::White;}
				catch(Exception^){textBox3->BackColor = Color::Red; error = true;}

				try{golfData->roi_y2 = Convert::ToInt32(textBox4->Text);
				textBox4->BackColor = Color::White;}
				catch(Exception^){textBox4->BackColor = Color::Red; error = true;}

				try{golfData->roi_height2 = Convert::ToInt32(textBox5->Text);
				textBox5->BackColor = Color::White;}
				catch(Exception^){textBox5->BackColor = Color::Red; error = true;}

				try{golfData->ballsize = Convert::ToInt32(textBox6->Text);
				textBox6->BackColor = Color::White;}
				catch(Exception^){textBox6->BackColor = Color::Red; error = true;}

				try{golfData->entrance_x = Convert::ToInt32(textBox7->Text);
				textBox7->BackColor = Color::White;}
				catch(Exception^){textBox7->BackColor = Color::Red; error = true;}

				try{golfData->entrance_y = Convert::ToInt32(textBox8->Text);
				textBox8->BackColor = Color::White;}
				catch(Exception^){textBox8->BackColor = Color::Red; error = true;}

				try{golfData->entrance_w = Convert::ToInt32(textBox9->Text);
				textBox9->BackColor = Color::White;}
				catch(Exception^){textBox9->BackColor = Color::Red; error = true;}

				try{golfData->entrance_h = Convert::ToInt32(textBox10->Text);
				textBox10->BackColor = Color::White;}
				catch(Exception^){textBox10->BackColor = Color::Red; error = true;}

				try{golfData->ballmaskscale = Convert::ToDouble(textBox11->Text);
				textBox11->BackColor = Color::White;}
				catch(Exception^){textBox11->BackColor = Color::Red; error = true;}

				try{golfData->cam1Offsetx = Convert::ToInt32(textBox21->Text);
				textBox21->BackColor = Color::White;}
				catch(Exception^){textBox21->BackColor = Color::Red; error = true;}

				try{golfData->cam1Offsety = Convert::ToInt32(textBox22->Text);
				textBox22->BackColor = Color::White;}
				catch(Exception^){textBox22->BackColor = Color::Red; error = true;}

				try{golfData->cam2Offsetx = Convert::ToInt32(textBox13->Text);
				textBox13->BackColor = Color::White;}
				catch(Exception^){textBox13->BackColor = Color::Red; error = true;}

				try{golfData->cam2Offsety = Convert::ToInt32(textBox14->Text);
				textBox14->BackColor = Color::White;}
				catch(Exception^){textBox14->BackColor = Color::Red; error = true;}

				try{golfData->cam3Offsetx = Convert::ToInt32(textBox15->Text);
				textBox15->BackColor = Color::White;}
				catch(Exception^){textBox15->BackColor = Color::Red; error = true;}

				try{golfData->cam3Offsety = Convert::ToInt32(textBox16->Text);
				textBox16->BackColor = Color::White;}
				catch(Exception^){textBox16->BackColor = Color::Red; error = true;}

				try{golfData->roi_y3 = Convert::ToInt32(textBox17->Text);
				textBox17->BackColor = Color::White;}
				catch(Exception^){textBox17->BackColor = Color::Red; error = true;}

				try{golfData->roi_height3 = Convert::ToInt32(textBox18->Text);
				textBox18->BackColor = Color::White;}
				catch(Exception^){textBox18->BackColor = Color::Red; error = true;}

				try{golfData->cam2Scale = Convert::ToDouble(textBox19->Text);
				textBox19->BackColor = Color::White;}
				catch(Exception^){textBox19->BackColor = Color::Red; error = true;}

				try{golfData->cam3Scale = Convert::ToDouble(textBox20->Text);
				textBox20->BackColor = Color::White;}
				catch(Exception^){textBox20->BackColor = Color::Red; error = true;}

				try{golfData->cam1thresh = Convert::ToByte(textBox23->Text);
				textBox23->BackColor = Color::White;}
				catch(Exception^){textBox23->BackColor = Color::Red; error = true;}

				try{golfData->cam2thresh = Convert::ToByte(textBox24->Text);
				textBox24->BackColor = Color::White;}
				catch(Exception^){textBox24->BackColor = Color::Red; error = true;}

				try{golfData->cam3thresh = Convert::ToByte(textBox25->Text);
				textBox25->BackColor = Color::White;}
				catch(Exception^){textBox25->BackColor = Color::Red; error = true;}


				array<Byte>^ bytes = gcnew array<Byte>(golfTransmissionConsts::optionsSize);
				IntPtr src((char*)golfData+golfTransmissionConsts::optionsOffset);
				System::Runtime::InteropServices::Marshal::Copy(src, bytes, 0, golfTransmissionConsts::optionsSize);
				try{System::IO::File::WriteAllBytes(gcnew String(settingFile), bytes);}
				catch(Exception^ e){MessageBox::Show(this, e->Message);}

				golfData->optionsClk = 1;
				return error;
			}
	private: System::Void setColorLow(System::Object^  sender, System::EventArgs^  e) {
				 colorDialog1->Color = panel1->BackColor;
				 colorDialog1->ShowDialog();
				 panel1->BackColor = colorDialog1->Color;
				 this->button2->Enabled = true;
			 }
	private: System::Void setColorHigh(System::Object^  sender, System::EventArgs^  e) {
				 colorDialog1->Color = panel2->BackColor;
				 colorDialog1->ShowDialog();
				 panel2->BackColor = colorDialog1->Color;
				 this->button2->Enabled = true;
			 }
	private: System::Void updateControls(System::Object^  sender, System::EventArgs^  e) {
				 if(this->Visible)
				 {
					 OptionsLoaded();
				 }
			 }
	private: System::Void cancelOptions(System::Object^  sender, System::EventArgs^  e) {
				 this->Hide();
			 }
	private: System::Void applyOptions(System::Object^  sender, System::EventArgs^  e) {
				 this->OptionsSaved();
				 this->button2->Enabled = false;
			 }
	private: System::Void allowApply(System::Object^  sender, System::EventArgs^  e) {
				 this->button2->Enabled = true;
			 }
};
}
