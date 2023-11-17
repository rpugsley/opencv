#include "opencv.c"



int main(int argc, char** argv)
{
    if (argc == 2 )
    {
		capture = cvCreateFileCapture( argv[1] );
		//captura e ajeita parametros para gravacao do video
	    fps = (int)cvGetCaptureProperty(capture,CV_CAP_PROP_FPS);
		codec = cvGetCaptureProperty( capture,CV_CAP_PROP_FOURCC);
	}
    if (argc == 1 )
    {
	    capture = cvCreateCameraCapture(0);
	    fps = 25;
		codec = CV_FOURCC('M','J','P','G');
        key=0;
		if(!capture)
			printf("\nCamera nao encontrada\n");

	}
	if(!capture)
	{
		printf("\nModo de uso : opencv.exe <nome do video.avi>\n");
		return -1;
	}

	size = cvSize((int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_WIDTH),(int)cvGetCaptureProperty( capture, CV_CAP_PROP_FRAME_HEIGHT));
	//cria a janela principal
    cvNamedWindow( "Video", CV_WINDOW_AUTOSIZE );


	printf("******************************************************************************");
	printf("\n\n\n\n\n\n\n\n\n\n");
	printf("                          Procurador de mudanca\n\n\n");
	printf("            Tecla d : Pausa , Play\n");
	printf("            Tecla b : volta para o primeiro frame\n");
	printf("            Tecla esc : sai do programa (com o video rodando)\n\n\n\n\n\n\n\n\n");
	printf("******************************************************************************");

	
	// cvCreateTrackbar("Sensibilidade","Video",&g_switch_value,20,NULL);


    //while principal
    while(1)                                                                                              
    {
	frame = cvQueryFrame( capture );
	if( !frame ) break;             
    
    cvShowImage( "Video", frame );
    if (argc != 1 )
    {
       cvCreateTrackbar("Frame","Video",&tempo_value,cvGetCaptureProperty( capture,CV_CAP_PROP_FRAME_COUNT),NULL);
       cvSetTrackbarPos("Frame","Video",cvGetCaptureProperty( capture,1));
    }
    cvCreateTrackbar("Sensibilidade","Video",&sensibilidade_switch_value,100,NULL);
    cvCreateTrackbar("Multiplicador","Video",&multiplicador_switch_value,10,NULL);
    cvCreateTrackbar("Atraso Inicio","Video",&atraso_fim_value,100/fps,NULL);
    cvCreateTrackbar("Atraso Fim","Video",&atraso_inicio_value,100/fps,NULL);
    cvCreateTrackbar("Troca ROI","Video",&trocaROI_value,9000/fps,NULL);

    //esc sai do programa, 'd' pausa , 'a' grava , 'b' retorna ao inicio
    if( key==27 ) 
        break;       
    if( key==100 ) 
    {
        while(1)
        {
             parado=1;
             cvSetMouseCallback("Video", mouseHandler, NULL);
             key=cvWaitKey(10);
             if( key==100 )
                 break;
             if( key==98 )
             {
                 cvSetCaptureProperty( capture,CV_CAP_PROP_POS_MSEC,0);
                 controle_atraso_inicio = controle_atraso_fim =0;
                 ntrecho =0;
              }
              if( key==49 )
              {
                 if(ntrecho==0)
                    break;
                 cvNamedWindow("Replay", CV_WINDOW_AUTOSIZE);
                 replay = cvCreateFileCapture((char*)&nomevideo);               
                 while(key!=27)
                 {
                   frameReplay = cvQueryFrame( replay );     
        		   cvShowImage("Replay", frameReplay );
                   if( !frameReplay ) 
                       cvSetCaptureProperty( replay,CV_CAP_PROP_POS_MSEC,0);
                   key=cvWaitKey(10);
                 }
                 cvReleaseCapture(&replay);
                 cvDestroyWindow("Replay");
                 key=100;
                 break;                
               }
              if( key==50 )
              { 
                 if(pegaNome(1)==0 || pegaNome==NULL)
                    break;
                 cvNamedWindow("Replay", CV_WINDOW_AUTOSIZE);
                 replay = cvCreateFileCapture((char*)pegaNome(1));               
                 while(key!=27)
                 {
                   frameReplay = cvQueryFrame( replay );     
        		   cvShowImage("Replay", frameReplay );
                   if( !frameReplay ) 
                       cvSetCaptureProperty( replay,CV_CAP_PROP_POS_MSEC,0);
                   key=cvWaitKey(10);
                 }
                 cvReleaseCapture(&replay);
                 cvDestroyWindow("Replay");
                 key=100;
                 break;                
               }
              if( key==51 )
              {
                 if(pegaNome(2)==0 || pegaNome==NULL)
                    break;
                 cvNamedWindow("Replay", CV_WINDOW_AUTOSIZE);
                 replay = cvCreateFileCapture((char*)pegaNome(2));               
                 while(key!=27)
                 {
                   frameReplay = cvQueryFrame( replay );     
        		   cvShowImage("Replay", frameReplay );
                   if( !frameReplay ) 
                       cvSetCaptureProperty( replay,CV_CAP_PROP_POS_MSEC,0);
                   key=cvWaitKey(10);
                 }
                 cvReleaseCapture(&replay);
                 cvDestroyWindow("Replay");
                 key=100;
                 break;                
               }
              if( key==52 )
              {
                 if(pegaNome(3)==0 || pegaNome==NULL)
                    break;
                 cvNamedWindow("Replay", CV_WINDOW_AUTOSIZE);
                 replay = cvCreateFileCapture((char*)pegaNome(3));               
                 while(key!=27)
                 {
                   frameReplay = cvQueryFrame( replay );     
        		   cvShowImage("Replay", frameReplay );
                   if( !frameReplay ) 
                       cvSetCaptureProperty( replay,CV_CAP_PROP_POS_MSEC,0);
                   key=cvWaitKey(10);
                 }
                 cvReleaseCapture(&replay);
                 cvDestroyWindow("Replay");
                 key=100;
                 break;                
               }
              if( key==53 )
              {
                 if(pegaNome(4)==0 || pegaNome==NULL)
                    break;
                 cvNamedWindow("Replay", CV_WINDOW_AUTOSIZE);
                 replay = cvCreateFileCapture((char*)pegaNome(4));               
                 while(key!=27)
                 {
                   frameReplay = cvQueryFrame( replay );     
        		   cvShowImage("Replay", frameReplay );
                   if( !frameReplay ) 
                       cvSetCaptureProperty( replay,CV_CAP_PROP_POS_MSEC,0);
                   key=cvWaitKey(10);
                 }
                 cvReleaseCapture(&replay);
                 cvDestroyWindow("Replay");
                 key=100;
                 break;                
               }
        }
		system("cls");
        parado=0;
    }
    if( key==98 )
	{
		system("cls");
		ntrecho =0;
        controle_atraso_inicio = controle_atraso_fim =0;
        cvSetCaptureProperty( capture,CV_CAP_PROP_POS_MSEC,0);
	}
    //puxa proximo frame              
	Gotoxy(0,0);
    if(ROI)
    {
		marcaROIframe(); //atualiza o roi do frame atual
		histograma1=criaHist(roi); //roi original
		histograma2=criaHist(areaMudancaROI); //roi atual

		//printf("\nComparacao   %f  --  %f  --  %f  --  %f  soma %f  \n",cvCompareHist(histograma1, histograma2,CV_COMP_CORREL),cvCompareHist(histograma1, histograma2,CV_COMP_CHISQR),cvCompareHist(histograma1, histograma2,CV_COMP_INTERSECT),cvCompareHist(histograma1, histograma2,CV_COMP_BHATTACHARYYA),cvCompareHist(histograma1, histograma2,CV_COMP_CORREL)+cvCompareHist(histograma1, histograma2,CV_COMP_CHISQR)+cvCompareHist(histograma1, histograma2,CV_COMP_INTERSECT)+cvCompareHist(histograma1, histograma2,CV_COMP_BHATTACHARYYA)-2);   
		comparacao = comparaHist(histograma1,histograma2) * cvGetTrackbarPos("Multiplicador","Video");
		if( comparacao >= cvGetTrackbarPos("Sensibilidade","Video"))
		{
			controle_atraso_fim =0;
            GravaVideo();
		}
		else
		{
            controle_atraso_inicio =0;
			if(!controleGravacao)
			{
				ParaVideo();
			}
		}

		cvReleaseHist(&histograma1);
		cvReleaseHist(&histograma2);
		printf("\n***********************   %.2f      *******************************\n",comparacao);
     }
     printf("\n\n\n\n\n\n Tempo %0.f s - CODEC  %0.f - FPS %i - Gravando?",(cvGetCaptureProperty( capture,0)/1000),cvGetCaptureProperty( capture,6),fps);
     printf(controleGravacao == 1 ? "Nao - " : "Sim - ");
     printf("Trechos  %i",ntrecho);

	 key=cvWaitKey(10);
     //cvWaitKey(50);
   }
   liberaMem(1);
    //system("PAUSE");
   system("cls");


   return 0; 
}

