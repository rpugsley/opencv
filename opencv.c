#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <string.h> 
#include "cv.h"
#include "highgui.h"

IplImage	*frame, *roi, *frameROI , *areaMudanca, *areaMudancaROI,*frameReplay;
CvPoint		point;
CvCapture	* capture, *replay;
CvVideoWriter *writer = NULL;
CvHistogram		*histograma1,*histograma2;
CvMat		*sig1,*sig2;
CvSize		size;
int		  sensibilidade_switch_value = 20, multiplicador_switch_value = 1, tempo_value = 0;
int       atraso_inicio_value = 0, atraso_fim_value =0;
int       controle_atraso_fim=0 , controle_atraso_inicio=0;
int       trocaROI_value=0, mudancaROI=0;
int       ultimo_valor_barra=0;
int       pos1,pos2,pos3,pos4;
int       drag = 0, ROI = 0, width, height;
int       parado=0;
int       controleGravacao =1;
char      nomevideo[100]= "gravacao.avi";
int       ntrecho=0; 
int       fps, codec, key=100;
double    ms=0;
int       h_bins = 30, s_bins = 32;
float	  comparacao=0;


void Gotoxy(int x, int y) 
{
   COORD coord;
   coord.X = x;
   coord.Y = y;
   SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}


CvHistogram *criaHist(IplImage* src )
{
// Compute the HSV image and decompose it into separate planes.
//
    IplImage* hsv = cvCreateImage( cvGetSize(src), 8, 3 );
    cvCvtColor( src, hsv, CV_BGR2HSV );
    IplImage* h_plane = cvCreateImage( cvGetSize(src), 8, 1 );
    IplImage* s_plane = cvCreateImage( cvGetSize(src), 8, 1 );
    IplImage* v_plane = cvCreateImage( cvGetSize(src), 8, 1 );
    IplImage* planes[] = { h_plane, s_plane };
    cvCvtPixToPlane( hsv, h_plane, s_plane, v_plane, 0 );
// Build the histogram and compute its contents.
//
    h_bins = 30; 
    s_bins = 32;
    CvHistogram* hist;
    {
    int hist_size[] = { h_bins, s_bins };
    float h_ranges[] = { 0, 360 }; // hue is [0,180]
    float s_ranges[] = { 0, 255 };
    float* ranges[] = { h_ranges, s_ranges };
    hist = cvCreateHist(
    2,
    hist_size,
    CV_HIST_ARRAY,
    ranges,
    1
    );
    }
    cvCalcHist( planes, hist, 0, 0 ); //Compute histogram
    cvNormalizeHist( hist, 1 ); //Normalize it
    
    cvReleaseImage(&hsv);
    cvReleaseImage(&h_plane);
    cvReleaseImage(&s_plane);
    cvReleaseImage(&v_plane);
    //cvReleaseImage(&planes[1]);
    //cvReleaseImage(&planes[0]);

    return hist;
}

float comparaHist(CvHistogram *hist1, CvHistogram *hist2 )
{
//Create matrices to store the signature in
    int numrows = h_bins*s_bins;
    int h,s;
    sig1 = cvCreateMat(numrows, 3, CV_32FC1); //1 count + 2 coords = 3
    sig2 = cvCreateMat(numrows, 3, CV_32FC1); //sigs are of type float.
//Fill signatures for the two histograms
//
    for( h = 0; h < h_bins; h++ ) 
    {
         for( s = 0; s < s_bins; s++ ) 
         {
            float bin_val = cvQueryHistValue_2D( hist1, h, s );
            cvSet2D(sig1,h*s_bins + s,0,cvScalar(bin_val,0,0,0)); //bin value
            cvSet2D(sig1,h*s_bins + s,1,cvScalar(h,0,0,0)); //Coord 1
            cvSet2D(sig1,h*s_bins + s,2,cvScalar(s,0,0,0)); //Coord 2
            bin_val = cvQueryHistValue_2D( hist2, h, s );
            cvSet2D(sig2,h*s_bins + s,0,cvScalar(bin_val,0,0,0)); //bin value
            cvSet2D(sig2,h*s_bins + s,1,cvScalar(h,0,0,0)); //Coord 1
            cvSet2D(sig2,h*s_bins + s,2,cvScalar(s,0,0,0)); //Coord 2
            }
   }
   // Do EMD AND REPORT
   //
  float emd = cvCalcEMD2(sig1,sig2,CV_DIST_L1,NULL,NULL,NULL,NULL,NULL); //utilizando manhattan
  //printf("\n%f       ****** %f  ********** %f *****\n",cvCalcEMD2(sig1,sig2,CV_DIST_L1,NULL,NULL,NULL,NULL,NULL),cvCalcEMD2(sig1,sig2,CV_DIST_L2,NULL,NULL,NULL,NULL,NULL),cvCalcEMD2(sig1,sig2,CV_DIST_C,NULL,NULL,NULL,NULL,NULL));

  cvReleaseMat(&sig1);
  cvReleaseMat(&sig2);  
  return emd;
}

void getNome()
{
     ntrecho++;
     ms = cvGetCaptureProperty( capture, 0)/1000;
     sprintf(nomevideo, "trecho_%i_%2.fseg.avi", ntrecho,ms );
     push(nomevideo); 
      
     //printf("%s",nomevideo);
}

//funcao de gravacao do video, controle para ter continuidade na gravacao
void GravaVideo()
{
     if(controle_atraso_inicio>=cvGetTrackbarPos("Atraso Inicio","Video")*fps)
     {
         if(controleGravacao==1)
         {
           getNome();
		   writer = cvCreateVideoWriter(nomevideo,codec,fps,size,1);     
           controleGravacao=0;
		   system("cls");
    	 }
    	 cvWriteFrame( writer, frame );
         mudancaROI++;
      }
     controle_atraso_inicio++;
     
}

void ParaVideo()
{    
    if(controle_atraso_fim >=(cvGetTrackbarPos("Atraso Fim","Video")*fps))
    {
        cvReleaseVideoWriter( &writer );
    	controleGravacao =1;
        Gotoxy(0,0);
        mudancaROI=0;
     }
     else
     {
         controle_atraso_fim++;
         cvWriteFrame( writer, frame );
      }
      
}
void liberaMem(int fim)
{
 //printf("%i     %i          %i", ROI,fim,writer);
     if(ROI)
     {
            cvDestroyWindow( "Area de mudanca");
            cvDestroyWindow( "ROI selecionado");

            cvReleaseImage(&frameROI);
         //   cvReleaseImage(&roiRGB);
            cvReleaseImage(&roi);
            cvReleaseImage(&areaMudancaROI);
            cvReleaseImage(&areaMudanca);
     }
     if(fim)
     {
            cvDestroyWindow( "Video");
       //forca gravacao do video no encerramento do programa 
           if(writer!= NULL)
                          cvReleaseVideoWriter( &writer );
            cvReleaseCapture( &capture );
       }
}


//handler do mouse, funciona somente com o video parado
//serve para marcar o roi
void mouseHandler(int event, int x, int y, int flags, void* param)
{

     if(!parado)
        return;
    /* user press left button */
    if (event == CV_EVENT_LBUTTONDOWN && !drag)
    {
        if(ROI)
               liberaMem(0);
        point = cvPoint(x, y);
        frameROI = cvCloneImage(frame);
        drag  = 1;
    }
 
    /* user drag the mouse */
    if (event == CV_EVENT_MOUSEMOVE && drag)
    {
        frame = cvCloneImage(frameROI); //imagem de auxilo para marcar o roi
        cvRectangle(
            frame,
            point,
            cvPoint(x, y),
            CV_RGB(255, 0, 0),
            1, 8, 0
        );
        cvShowImage("Video", frame);
    }

    /* user release left button */
    if (event == CV_EVENT_LBUTTONUP && drag)
    {
        roi = cvCloneImage(frameROI);
        frame = cvCloneImage(frameROI);
        
        if(point.x < x && point.y < y)
        {
          pos1 = point.x;
          pos2 = point.y;
          pos3 = x - point.x;
          pos4 = y - point.y;
        cvSetImageROI(
            roi,
            cvRect(
                point.x,
                point.y,
                x - point.x,
                y - point.y
            )
        );}
        if(point.x > x && point.y < y)
        {
          pos1 = point.x;
          pos2 = point.y;
          pos3 = point.x - x;
          pos4 = y - point.y;

        cvSetImageROI(
            roi,
            cvRect(
                point.x,
                point.y,
                point.x - x,
                y - point.y
            )
        );}
        if(point.x < x && point.y > y)
        {
          pos1 = point.x;
          pos2 = point.y;
          pos3 = x - point.x;
          pos4 = point.y - y;
        cvSetImageROI(
            roi,
            cvRect(
                point.x,
                point.y,
                x - point.x,
                point.y - y
            )
        );}
        if(point.x > x && point.y > y)
        {
          pos1 = point.x;
          pos2 = point.y;
          pos3 = point.x -x;
          pos4 = point.y - y;
        cvSetImageROI(
            roi,
            cvRect(
                point.x,
                point.y,
                point.x - x,
                point.y - y
            )
        );}

		ROI = 1;           //exibicao da tela
		cvNamedWindow("ROI selecionado", CV_WINDOW_AUTOSIZE);
		cvShowImage("ROI selecionado", roi);	
		cvShowImage("Video", frame);
		drag = 0;
		

	}
  /* user click right button: reset all */
    if (event == CV_EVENT_RBUTTONDOWN)
    {  
        if(ROI)
        {
       // cvResetImageROI(roiRGB);        
        liberaMem(0);
        drag = 0;
        ROI=0;
        }

    }
}



void mudaROI()
{    
     if(cvGetTrackbarPos("Troca ROI","Video")!= ultimo_valor_barra)
     {        
        mudancaROI=0;
        ultimo_valor_barra = cvGetTrackbarPos("Troca ROI","Video");
     }
                                        
     if(mudancaROI != 0 && mudancaROI == cvGetTrackbarPos("Troca ROI","Video")*fps)
     {
           roi = cvCloneImage(frame);
           cvSetImageROI(
            roi,
            cvRect(
                pos1,
                pos2,
                pos3,
                pos4
                )
            );
            cvShowImage("ROI selecionado", roi);
            mudancaROI=0;
            ultimo_valor_barra = cvGetTrackbarPos("Troca ROI","Video");
      }   
}


//desenha o roi em cada frame novo
void marcaROIframe()
{
     cvReleaseImage(&areaMudancaROI);
    
     areaMudanca = cvCloneImage(frame);
        cvSetImageROI(
            areaMudanca,
            cvRect(
                pos1,
                pos2,
                pos3,
                pos4
            )
        );

        cvNamedWindow("Area de mudanca", CV_WINDOW_AUTOSIZE);      
		cvShowImage("Area de mudanca", areaMudanca);
        areaMudancaROI = cvCloneImage( areaMudanca );       

		

        cvResetImageROI(areaMudanca);
        cvReleaseImage(&areaMudanca);  
        if(cvGetTrackbarPos("Troca ROI","Video")!=0)
           mudaROI();
     
            
}//alterado para melhor perfomance em relacao a memoria



   
            
         
