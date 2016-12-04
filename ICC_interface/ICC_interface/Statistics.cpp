#include "stdafx.h"
#include "StatisticsWindow.h"

namespace ICC_interface
{
	StatisticsWindow::StatisticsWindow(void)
	{
		InitializeComponent();
		BallCounts = gcnew List<int>();

		try{
			FileStream^ datafile = File::Open(L"brands.txt", FileMode::Open);
			StreamReader^ stream = gcnew StreamReader(datafile);
			
			while(!stream->EndOfStream)
			{
				BallCounts->Add(0);
				Series^ s = gcnew Series(stream->ReadLine(), 0);
				chart1->Series->Add(s);
				s->Points->AddY(0);
			}
			datafile->Close();
		}
		catch(Exception^ e){MessageBox::Show(this,L"Error loading brands file: " + e->Message);}
		
		load();
	}

	void StatisticsWindow::addBall(int ball)
	{
		if(ball < BallCounts->Count)
		{
			Series^ s = chart1->Series[ball];
			BallCounts[ball] += 1;
			s->Points->Clear();
			s->Points->AddY(BallCounts[ball]);
		}
	}

	void StatisticsWindow::setBall(int ball, int num)
	{
		if(ball < BallCounts->Count)
		{
			Series^ s = chart1->Series[ball];
			BallCounts[ball] = num;
			s->Points->Clear();
			s->Points->AddY((double)num);
		}
	}

	void StatisticsWindow::save()
	{
		try
		{
			FileStream^ F = File::Open(L"statistics.txt", FileMode::OpenOrCreate);
			StreamWriter^ writer = gcnew StreamWriter(F);
			for(int i = 0; i<BallCounts->Count; ++i)
			{
				Series^ s = chart1->Series[i];
				String^ str = s->Name + L"," + BallCounts[i] + L"\n";
				writer->Write(str);
			}
			writer->Flush();
			F->Close();
		}
		catch(Exception^)
		{
		}
	}

	void StatisticsWindow::load()
	{
		FileStream^ F;
		try
		{
			F = File::Open(L"statistics.txt", FileMode::OpenOrCreate);
			StreamReader^ reader = gcnew StreamReader(F);
			for(int i = 0; i<BallCounts->Count; ++i)
			{
				String^ r = reader->ReadLine();
				array<wchar_t>^ delim = {L','};
				array<String^>^ split = r->Split( delim );
				if(split->Length > 1)
				{
					setBall(i, Convert::ToInt32(split[1]));
				}
			}
		}
		catch(Exception^){}
		F->Close();
	}
}