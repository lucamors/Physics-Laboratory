void compton_errorifit()
{
//=========Macro generated from canvas: c1/c1
//=========  (Fri Apr 13 16:08:36 2018) by ROOT version 6.13/01
   TCanvas *c1 = new TCanvas("c1", "c1",0,23,1105,777);
   c1->Range(-12.99375,-68.77977,107.0438,594.8611);
   c1->SetFillColor(0);
   c1->SetBorderMode(0);
   c1->SetBorderSize(2);
   c1->SetFrameBorderMode(0);
   c1->SetFrameBorderMode(0);
   
   Double_t Graph0_fx1001[6] = {
   0,
   20,
   39.89675,
   59.92114,
   70,
   90};
   Double_t Graph0_fy1001[6] = {
   8.392,
   18.87,
   87.63092,
   154.7903,
   181,
   232.2};
   Double_t Graph0_fex1001[6] = {
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph0_fey1001[6] = {
   0.84,
   4.02,
   1.41,
   1.1,
   1.4,
   1.2};
   TGraphErrors *gre = new TGraphErrors(6,Graph0_fx1001,Graph0_fy1001,Graph0_fex1001,Graph0_fey1001);
   gre->SetName("Graph0");
   gre->SetTitle("Experimental Electron Energy");
   gre->SetFillStyle(1000);
   gre->SetLineColor(4);

   Int_t ci;      // for color index setting
   TColor *color; // for color definition with alpha
   ci = TColor::GetColor("#0000ff");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(3);
   
   TH1F *Graph_Graph01001 = new TH1F("Graph_Graph01001","Experimental Electron Energy",100,0,99);
   Graph_Graph01001->SetMinimum(-2.415676);
   Graph_Graph01001->SetMaximum(528.497);
   Graph_Graph01001->SetDirectory(0);
   Graph_Graph01001->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph01001->SetLineColor(ci);
   Graph_Graph01001->GetXaxis()->SetTitle("Angle #theta");
   Graph_Graph01001->GetXaxis()->SetRange(0,96);
   Graph_Graph01001->GetXaxis()->SetLabelFont(42);
   Graph_Graph01001->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph01001->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph01001->GetXaxis()->SetTitleFont(42);
   Graph_Graph01001->GetYaxis()->SetTitle("Energy [keV]");
   Graph_Graph01001->GetYaxis()->SetLabelFont(42);
   Graph_Graph01001->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph01001->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph01001->GetYaxis()->SetTitleOffset(0);
   Graph_Graph01001->GetYaxis()->SetTitleFont(42);
   Graph_Graph01001->GetZaxis()->SetLabelFont(42);
   Graph_Graph01001->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph01001->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph01001->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph01001);
   
   gre->Draw("AP");
   
   Double_t Graph1_fx1[100] = {
   0,
   0.9,
   1.8,
   2.7,
   3.6,
   4.5,
   5.4,
   6.3,
   7.2,
   8.1,
   9,
   9.9,
   10.8,
   11.7,
   12.6,
   13.5,
   14.32718,
   15.3,
   16.2,
   17.1,
   18,
   18.9,
   19.8,
   20.7,
   21.6,
   22.5,
   23.4,
   24.3,
   25.2,
   26.1,
   27,
   27.9,
   28.8,
   29.7,
   30.6,
   31.5,
   32.4,
   33.3,
   34.2,
   35.1,
   36,
   36.9,
   37.8,
   38.7,
   39.6,
   40.5,
   41.4,
   42.3,
   43.2,
   44.1,
   45,
   45.9,
   46.8,
   47.7,
   48.6,
   49.5,
   50.4,
   51.3,
   52.2,
   53.1,
   54,
   54.9,
   55.8,
   56.7,
   57.6,
   58.5,
   59.4,
   60.3,
   61.2,
   62.1,
   63,
   63.9,
   64.8,
   65.7,
   66.6,
   67.5,
   68.4,
   69.3,
   70.2,
   71.1,
   72,
   72.8717,
   73.8,
   74.7,
   75.6,
   76.5,
   77.4,
   78.3,
   79.2,
   80.05436,
   81,
   81.9,
   82.8,
   83.7,
   84.6,
   85.5,
   86.4,
   87.3,
   88.2,
   89.1};
   Double_t Graph1_fy1[100] = {
   0,
   0.06303303,
   0.2520233,
   0.5666448,
   1.006356,
   1.570401,
   2.257816,
   3.067427,
   3.997863,
   5.047555,
   6.214744,
   7.497492,
   8.893683,
   10.40104,
   12.01712,
   13.73935,
   15.79745,
   17.49122,
   19.51506,
   21.63342,
   23.84315,
   26.141,
   28.52363,
   30.98766,
   33.52965,
   36.1461,
   38.83351,
   41.58834,
   44.40703,
   47.28604,
   50.22182,
   53.21084,
   56.24959,
   59.3346,
   62.46243,
   65.62969,
   68.83305,
   72.06921,
   75.33495,
   78.62713,
   81.94266,
   85.27854,
   88.63184,
   91.99972,
   95.37942,
   98.76826,
   102.1637,
   105.5631,
   108.9643,
   112.3648,
   115.7624,
   119.155,
   122.5406,
   125.9172,
   129.283,
   132.6362,
   135.9751,
   139.298,
   142.6036,
   145.8904,
   149.157,
   152.4022,
   155.6247,
   158.8235,
   161.9976,
   165.1459,
   168.2675,
   171.3616,
   174.4275,
   177.4644,
   180.4716,
   183.4487,
   186.3949,
   189.31,
   192.1933,
   195.0445,
   197.8634,
   200.6494,
   203.4025,
   206.1224,
   208.8089,
   212.2293,
   214.0811,
   216.6667,
   219.2184,
   221.7364,
   224.2205,
   226.6708,
   229.0874,
   231.6701,
   233.8197,
   236.1356,
   238.4182,
   240.6676,
   242.884,
   245.0676,
   247.2185,
   249.337,
   251.4232,
   253.4775};
   TGraph *graph = new TGraph(100,Graph1_fx1,Graph1_fy1);
   graph->SetName("Graph1");
   graph->SetTitle("Theoretical Electron Energy");
   graph->SetFillStyle(1000);
   graph->SetLineColor(4);
   graph->SetLineWidth(2);
   
   TH1F *Graph_Graph11 = new TH1F("Graph_Graph11","Theoretical Electron Energy",100,0,98.01);
   Graph_Graph11->SetMinimum(0);
   Graph_Graph11->SetMaximum(278.8253);
   Graph_Graph11->SetDirectory(0);
   Graph_Graph11->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph11->SetLineColor(ci);
   Graph_Graph11->GetXaxis()->SetLabelFont(42);
   Graph_Graph11->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetXaxis()->SetTitleFont(42);
   Graph_Graph11->GetYaxis()->SetLabelFont(42);
   Graph_Graph11->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetYaxis()->SetTitleOffset(0);
   Graph_Graph11->GetYaxis()->SetTitleFont(42);
   Graph_Graph11->GetZaxis()->SetLabelFont(42);
   Graph_Graph11->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph11->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph11->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph11);
   
   graph->Draw("");
   
   Double_t Graph2_fx2[100] = {
   0,
   0.9,
   1.8,
   2.7,
   3.6,
   4.5,
   5.4,
   6.3,
   7.2,
   8.1,
   8.941862,
   9.9,
   10.8,
   11.7,
   12.6,
   13.5,
   14.4,
   15.3,
   16.2,
   17.1,
   18,
   18.9,
   19.8,
   20.7,
   21.6,
   22.5,
   23.4,
   24.3,
   25.2,
   26.1,
   27,
   27.9,
   28.8,
   29.7,
   30.6,
   31.5,
   32.4,
   33.3,
   34.2,
   35.1,
   36,
   36.9,
   37.8,
   38.7,
   39.6,
   40.5,
   41.4,
   42.3,
   43.2,
   44.1,
   45,
   45.9,
   46.8,
   47.7,
   48.6,
   49.5,
   50.4,
   51.21489,
   52.2,
   53.1,
   54,
   54.9,
   55.8,
   56.7,
   57.6,
   58.5,
   59.4,
   60.3,
   61.2,
   62.1,
   63,
   63.9,
   64.8,
   65.7,
   66.6,
   67.5,
   68.4,
   69.3,
   70.2,
   71.1,
   72,
   72.9,
   73.8,
   74.7,
   75.6,
   76.5,
   77.4,
   78.3,
   79.2,
   80.1,
   81,
   81.9,
   82.8,
   83.7,
   84.6,
   85.5,
   86.4,
   87.3,
   88.2,
   89.1};
   Double_t Graph2_fy2[100] = {
   511,
   510.937,
   510.748,
   510.4334,
   509.9936,
   509.4296,
   508.7422,
   507.9326,
   507.0021,
   505.9524,
   504.8199,
   503.5025,
   502.1063,
   500.599,
   498.9829,
   497.2606,
   495.435,
   493.5088,
   491.4849,
   489.3666,
   487.1568,
   484.859,
   482.4764,
   480.0123,
   477.4704,
   474.8539,
   472.1665,
   469.4117,
   466.593,
   463.714,
   460.7782,
   457.7892,
   454.7504,
   451.6654,
   448.5376,
   445.3703,
   442.167,
   438.9308,
   435.665,
   432.3729,
   429.0573,
   425.7215,
   422.3682,
   419.0003,
   415.6206,
   412.2317,
   408.8363,
   405.4369,
   402.0357,
   398.6352,
   395.2376,
   391.845,
   388.4594,
   385.0828,
   381.717,
   378.3638,
   375.0249,
   372.1747,
   368.3964,
   365.1096,
   361.843,
   358.5978,
   355.3753,
   352.1765,
   349.0024,
   345.8541,
   342.7325,
   339.6384,
   336.5725,
   333.5356,
   330.5284,
   327.5513,
   324.6051,
   321.69,
   318.8067,
   315.9555,
   313.1366,
   310.3506,
   307.5975,
   304.8776,
   302.1911,
   299.5381,
   296.9189,
   294.3333,
   291.7816,
   289.2636,
   286.7795,
   284.3292,
   281.9126,
   279.5296,
   277.1803,
   274.8644,
   272.5818,
   270.3324,
   268.116,
   265.9324,
   263.7815,
   261.663,
   259.5768,
   257.5225};
   graph = new TGraph(100,Graph2_fx2,Graph2_fy2);
   graph->SetName("Graph2");
   graph->SetTitle("Theoretical Photon Energy");
   graph->SetFillStyle(1000);
   graph->SetLineColor(2);
   graph->SetLineStyle(4);
   graph->SetLineWidth(2);
   
   TH1F *Graph_Graph22 = new TH1F("Graph_Graph22","Theoretical Photon Energy",100,0,98.01);
   Graph_Graph22->SetMinimum(232.1747);
   Graph_Graph22->SetMaximum(536.3478);
   Graph_Graph22->SetDirectory(0);
   Graph_Graph22->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph22->SetLineColor(ci);
   Graph_Graph22->GetXaxis()->SetLabelFont(42);
   Graph_Graph22->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph22->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph22->GetXaxis()->SetTitleFont(42);
   Graph_Graph22->GetYaxis()->SetLabelFont(42);
   Graph_Graph22->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph22->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph22->GetYaxis()->SetTitleOffset(0);
   Graph_Graph22->GetYaxis()->SetTitleFont(42);
   Graph_Graph22->GetZaxis()->SetLabelFont(42);
   Graph_Graph22->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph22->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph22->GetZaxis()->SetTitleFont(42);
   graph->SetHistogram(Graph_Graph22);
   
   graph->Draw("");
   
   Double_t Graph3_fx1002[6] = {
   -0.0431961,
   19.87236,
   39.89675,
   59.92114,
   69.93334,
   89.8489};
   Double_t Graph3_fy1002[6] = {
   497.6567,
   480.8668,
   424.3116,
   357.1522,
   329.7582,
   278.505};
   Double_t Graph3_fex1002[6] = {
   0,
   0,
   0,
   0,
   0,
   0};
   Double_t Graph3_fey1002[6] = {
   0.6,
   1.2,
   1.6,
   1.3,
   1.5,
   0.3};
   gre = new TGraphErrors(6,Graph3_fx1002,Graph3_fy1002,Graph3_fex1002,Graph3_fey1002);
   gre->SetName("Graph3");
   gre->SetTitle("Experimental Photon Energy");
   gre->SetFillStyle(1000);
   gre->SetLineColor(2);

   ci = TColor::GetColor("#ff0000");
   gre->SetMarkerColor(ci);
   gre->SetMarkerStyle(3);
   
   TH1F *Graph_Graph31002 = new TH1F("Graph_Graph31002","Experimental Photon Energy",100,0,99);
   Graph_Graph31002->SetMinimum(255.6);
   Graph_Graph31002->SetMaximum(519.6);
   Graph_Graph31002->SetDirectory(0);
   Graph_Graph31002->SetStats(0);

   ci = TColor::GetColor("#000099");
   Graph_Graph31002->SetLineColor(ci);
   Graph_Graph31002->GetXaxis()->SetLabelFont(42);
   Graph_Graph31002->GetXaxis()->SetLabelSize(0.035);
   Graph_Graph31002->GetXaxis()->SetTitleSize(0.035);
   Graph_Graph31002->GetXaxis()->SetTitleFont(42);
   Graph_Graph31002->GetYaxis()->SetLabelFont(42);
   Graph_Graph31002->GetYaxis()->SetLabelSize(0.035);
   Graph_Graph31002->GetYaxis()->SetTitleSize(0.035);
   Graph_Graph31002->GetYaxis()->SetTitleOffset(0);
   Graph_Graph31002->GetYaxis()->SetTitleFont(42);
   Graph_Graph31002->GetZaxis()->SetLabelFont(42);
   Graph_Graph31002->GetZaxis()->SetLabelSize(0.035);
   Graph_Graph31002->GetZaxis()->SetTitleSize(0.035);
   Graph_Graph31002->GetZaxis()->SetTitleFont(42);
   gre->SetHistogram(Graph_Graph31002);
   
   gre->Draw("P");
   
   TLegend *leg = new TLegend(0.6210335,0.7842876,0.893019,0.8881491,NULL,"brNDC");
   leg->SetBorderSize(1);
   leg->SetLineColor(1);
   leg->SetLineStyle(1);
   leg->SetLineWidth(1);
   leg->SetFillColor(0);
   leg->SetFillStyle(1001);
   TLegendEntry *entry=leg->AddEntry("Graph0","Experimental Electron Energy","lpf");
   entry->SetFillStyle(1000);
   entry->SetLineColor(4);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#0000ff");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(3);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph1","Theoretical Electron Energy","lpflpf");
   entry->SetFillStyle(1000);
   entry->SetLineColor(4);
   entry->SetLineStyle(1);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(1);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph2","Theoretical Photon Energy","lpflpflpf");
   entry->SetFillStyle(1000);
   entry->SetLineColor(2);
   entry->SetLineStyle(4);
   entry->SetLineWidth(2);
   entry->SetMarkerColor(1);
   entry->SetMarkerStyle(1);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   entry=leg->AddEntry("Graph3","Experimental Photon Energy","lpflpflpflpf");
   entry->SetFillStyle(1000);
   entry->SetLineColor(2);
   entry->SetLineStyle(1);
   entry->SetLineWidth(1);

   ci = TColor::GetColor("#ff0000");
   entry->SetMarkerColor(ci);
   entry->SetMarkerStyle(3);
   entry->SetMarkerSize(1);
   entry->SetTextFont(42);
   leg->Draw();
   
   TPaveText *pt = new TPaveText(0.2919311,0.9360852,0.8159565,0.9973369,"blNDC");
   pt->SetName("title");
   pt->SetBorderSize(0);
   pt->SetFillColor(0);
   pt->SetFillStyle(0);
   pt->SetLineColor(0);
   pt->SetTextFont(42);
   pt->SetTextSize(0.001331558);
   TText *pt_LaTex = pt->AddText("Experimental Electron Energy");
   pt->Draw();
   c1->Modified();
   c1->cd();
   c1->SetSelected(c1);
}
