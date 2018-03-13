
void back_sub(string filename_noscat, string filename_scat)
{
	TFile * file_noscat = new TFile(filename_noscat.c_str());
	TFile * file_scat = new TFile(filename_scat.c_str());

	TH1F * noscat_h = (TH1F *) file_noscat->Get("ch2");
	TH1F * scat_h = (TH1F *) file_scat->Get("ch2");

	double current_bg, current_wbg;

	for (size_t i = 0; i < 3500; i++)
	{
		current_bg = 1.9235955 * noscat_h->GetBinContent(i);
		current_wbg = scat_h->GetBinContent(i);

		scat_h->SetBinContent(i, current_wbg - current_bg );
	}

	scat_h->Draw();
	double gamma=  scat_h->Integral(scat_h->GetXaxis()->FindBin(215.5),scat_h->GetXaxis()->FindBin(295.5));
  cout<<gamma<<endl;
	return ;
}
