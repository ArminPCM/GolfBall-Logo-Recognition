#pragma once

namespace ICC_interface {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::Collections::Generic;
	using namespace System::IO;
	using namespace System::Windows::Forms::DataVisualization::Charting;

	/// <summary>
	/// Summary for StatisticsWindow
	/// </summary>
	public ref class StatisticsWindow : public System::Windows::Forms::Form
	{
	public:
		StatisticsWindow(void);

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~StatisticsWindow()
		{
			if (components)
			{
				delete components;
			}
		}

	private: System::Windows::Forms::DataVisualization::Charting::Chart^  chart1;
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
			System::Windows::Forms::DataVisualization::Charting::ChartArea^  chartArea1 = (gcnew System::Windows::Forms::DataVisualization::Charting::ChartArea());
			System::Windows::Forms::DataVisualization::Charting::Legend^  legend1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Legend());
			System::ComponentModel::ComponentResourceManager^  resources = (gcnew System::ComponentModel::ComponentResourceManager(StatisticsWindow::typeid));
			this->chart1 = (gcnew System::Windows::Forms::DataVisualization::Charting::Chart());
			this->button1 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->BeginInit();
			this->SuspendLayout();
			// 
			// chart1
			// 
			chartArea1->Name = L"ChartArea1";
			this->chart1->ChartAreas->Add(chartArea1);
			this->chart1->Dock = System::Windows::Forms::DockStyle::Fill;
			legend1->Name = L"Legend1";
			this->chart1->Legends->Add(legend1);
			this->chart1->Location = System::Drawing::Point(0, 0);
			this->chart1->Name = L"chart1";
			this->chart1->Size = System::Drawing::Size(490, 457);
			this->chart1->TabIndex = 0;
			this->chart1->Text = L"chart1";
			// 
			// button1
			// 
			this->button1->Anchor = static_cast<System::Windows::Forms::AnchorStyles>((System::Windows::Forms::AnchorStyles::Bottom | System::Windows::Forms::AnchorStyles::Right));
			this->button1->Location = System::Drawing::Point(403, 422);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(75, 23);
			this->button1->TabIndex = 1;
			this->button1->Text = L"Reset";
			this->button1->UseVisualStyleBackColor = true;
			this->button1->Click += gcnew System::EventHandler(this, &StatisticsWindow::ResetStats);
			// 
			// StatisticsWindow
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(490, 457);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->chart1);
			this->Icon = (cli::safe_cast<System::Drawing::Icon^  >(resources->GetObject(L"$this.Icon")));
			this->Name = L"StatisticsWindow";
			this->Text = L"Statistics";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &StatisticsWindow::cancelClose);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^  >(this->chart1))->EndInit();
			this->ResumeLayout(false);

		}
#pragma endregion
	private: System::Void cancelClose(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e) {
				 if (e->CloseReason != CloseReason::UserClosing) return;
				 this->Hide();
				 e->Cancel = true;
			 }
	private: System::Collections::Generic::List<int>^ BallCounts;
	public: void addBall(int ball);
	public: void setBall(int ball, int num);
	public: void save();
	public: void load();
	private: System::Void ResetStats(System::Object^  sender, System::EventArgs^  e) {
				 for(int i = 0; i<BallCounts->Count; ++i)
				 {
					 setBall(i, 0);
				 }
			 }
	};
}
