#include "stdio.h"
#include "stdlib.h" 
class wind_c						//На основе	этой структуры рисуются	окна (все)
{
private:
	HFONT font;					//Используемы шрифт
	int fontsize;
public:
	HDC dc_buf;	//Это сами буфера
	HBITMAP bit_buf;
	HGDIOBJ pbit_buf;	//Это все для буферов

	int	top;					//Позиция с верху
	int	left;					//позиция с лева
	int	width;					//Ширина
	int	height;					//Высота
	char name[255];				//Надпись
	bool visible;				//Видимость
	HPEN cvetbord;				//Кисть окантовки
	HBRUSH cvetzal;				//Кисть заливки
	COLORREF fontcolor;			//Цвет надписи
	COLORREF bkcolor;			//Цвет под надписью
	HPEN pen;					//Кисть, которой на ней рисуют
	HBRUSH brush;				//Кисть, которой ее закрашивают
//	HDC *ptr_bitmap;			//Буфер, на который все будит сохраняться

	void setfontsize(int size)
	{
		fontsize=size;
		font=CreateFont(fontsize/*nHeight*/,
				0/*nWidth*/,
				0/*nEscapement*/,
				0/*nOrientation*/,
				FW_NORMAL/*nWeight*/,
				FALSE/*bItalic*/,
				FALSE/*bUnderline*/,
				0/*cStrikeOut*/,
				RUSSIAN_CHARSET/*nCharSet*/,
				OUT_DEFAULT_PRECIS/*nOutPrecision*/,
				CLIP_DEFAULT_PRECIS/*nClipPrecision*/,
				DEFAULT_QUALITY/*nQuality*/,
				DEFAULT_PITCH |	FF_SWISS/*nPitchAndFamily*/,
			"Arial Cyr"/*lpszFacename*/);
	}
	
	wind_c ()
	{
		strcpy(name, "Окно");
		visible=true;
		bkcolor=RGB(0,0,0);
		cvetbord=CreatePen(0,1,bkcolor);
		cvetzal=CreateSolidBrush(bkcolor);
		pen=CreatePen(0,1,RGB(0,100,0));
		brush=CreateSolidBrush(RGB(0,100,0));
		fontcolor=RGB(20,170,20);
		setfontsize(14);
	}
	

	bool in(int x,int y)
	{
		if(x>=left &&x<=left+width && y>=top && y<=top+height)
			return true;
		else return false;
	}

	void initgraf(HDC Out) //Инициалезирует набор кистей для рисования на Out
	{
		SelectObject(Out,pen);
		SelectObject(Out,brush);
		SetBkColor(Out,bkcolor);
		SelectObject(Out,font);
		SetTextColor(Out,fontcolor);
	}

	void clear()
	{
		SelectObject(dc_buf,cvetbord);
		SelectObject(dc_buf,cvetzal);
		Rectangle(dc_buf,0,0,width,height);
	}
	
	void paint(HDC Out) // Нарисовать окно
	{		
		if (visible)
		{
			SetBkColor(dc_buf,bkcolor);
			SelectObject(dc_buf,font);
			SetTextColor(dc_buf,fontcolor);
			TextOut(dc_buf,10,10,name,int(strlen(name)));
			BitBlt(Out,left,top,left+width,top+height,dc_buf,0,0,SRCCOPY);
		}
	}

	void init_dc(HWND hWnd) //Переразметка буфера окна
	{
		DeleteDC(dc_buf);											//Удаление буфера
		DeleteObject(bit_buf);
		dc_buf=CreateCompatibleDC(GetDC(hWnd));
		bit_buf=CreateCompatibleBitmap(GetDC(hWnd),width,height);
		SelectObject(dc_buf,bit_buf);
		pbit_buf=SelectObject(dc_buf,bit_buf);
		RECT winr;
		winr.left=0;
		winr.top=0;
		winr.bottom=height;
		winr.right=width;

		FillRect(dc_buf,&winr,cvetzal);
	}
};

struct button_c					//На основе	этой структуры рисуются	кнопки (все)
{

public:
	int	top;					//Позиция с верху
	int	left;					//позиция с лева
	int	width;					//Ширина
	int	height;					//Высота
	char name[255];				//Надпись
	bool visible;				//Видимость
	bool enabel;				//"Активность"
	HPEN cvetbord;				//Кисть окантовки
	HBRUSH cvetzal;				//Кисть заливки
	
	COLORREF fontcolor;			//Цвет надписи
	COLORREF bkcolor;			//Цвет под надписью
	HPEN ecvetbord;				//Если "Активно", Кисть окантовки
	HBRUSH ecvetzal;			//Если "Активно", Кисть заливки
	HFONT efont;				//Если "Активно", Используемы шрифт
	COLORREF efontcolor;		//Если "Активно", Цвет надписи
	COLORREF ebkcolor;			//Если "Активно", Цвет под надписью
private:
	HFONT font;					//Используемы шрифт
	int fontsize;
public :
	button_c()
	{
		top=0;
		left=0;
		width=100;
		height=20;
		strcpy(name,"Кнопка");
		visible=true;
		enabel=true;
		ecvetbord=CreatePen(0,2,RGB(0,100,0));
		efontcolor=RGB(0,0,0);
		ebkcolor=RGB(200,200,200);
		fontsize=14;
		ecvetzal=CreateSolidBrush(ebkcolor);
		efont=CreateFont(fontsize/*nHeight*/,
			0/*nWidth*/,
			0/*nEscapement*/,
			0/*nOrientation*/,
			FW_NORMAL/*nWeight*/,
			FALSE/*bItalic*/,
			FALSE/*bUnderline*/,
			0/*cStrikeOut*/,
			RUSSIAN_CHARSET/*nCharSet*/,
			OUT_DEFAULT_PRECIS/*nOutPrecision*/,
			CLIP_DEFAULT_PRECIS/*nClipPrecision*/,
			DEFAULT_QUALITY/*nQuality*/,
			DEFAULT_PITCH |	FF_SWISS/*nPitchAndFamily*/,
			"Arial Cyr"/*lpszFacename*/);
		fontcolor=RGB(200,200,200);
		bkcolor=RGB(50,50,50);
		cvetbord=CreatePen(0,1,RGB(0,0,0));
		cvetzal=CreateSolidBrush(bkcolor);
		font=CreateFont(fontsize/*nHeight*/,
				0/*nWidth*/,
				0/*nEscapement*/,
				0/*nOrientation*/,
				FW_NORMAL/*nWeight*/,
				FALSE/*bItalic*/,
				FALSE/*bUnderline*/,
				0/*cStrikeOut*/,
				RUSSIAN_CHARSET/*nCharSet*/,
				OUT_DEFAULT_PRECIS/*nOutPrecision*/,
				CLIP_DEFAULT_PRECIS/*nClipPrecision*/,
				DEFAULT_QUALITY/*nQuality*/,
				DEFAULT_PITCH |	FF_SWISS/*nPitchAndFamily*/,
			"Arial Cyr"/*lpszFacename*/);

			
	}
	
	void setfontsize(int size)
	{
		fontsize=size;
		font=CreateFont(fontsize/*nHeight*/,
				0/*nWidth*/,
				0/*nEscapement*/,
				0/*nOrientation*/,
				FW_NORMAL/*nWeight*/,
				FALSE/*bItalic*/,
				FALSE/*bUnderline*/,
				0/*cStrikeOut*/,
				RUSSIAN_CHARSET/*nCharSet*/,
				OUT_DEFAULT_PRECIS/*nOutPrecision*/,
				CLIP_DEFAULT_PRECIS/*nClipPrecision*/,
				DEFAULT_QUALITY/*nQuality*/,
				DEFAULT_PITCH |	FF_SWISS/*nPitchAndFamily*/,
			"Arial Cyr"/*lpszFacename*/);
		efont=CreateFont(fontsize/*nHeight*/,
			0/*nWidth*/,
			0/*nEscapement*/,
			0/*nOrientation*/,
			FW_NORMAL/*nWeight*/,
			FALSE/*bItalic*/,
			FALSE/*bUnderline*/,
			0/*cStrikeOut*/,
			RUSSIAN_CHARSET/*nCharSet*/,
			OUT_DEFAULT_PRECIS/*nOutPrecision*/,
			CLIP_DEFAULT_PRECIS/*nClipPrecision*/,
			DEFAULT_QUALITY/*nQuality*/,
			DEFAULT_PITCH |	FF_SWISS/*nPitchAndFamily*/,
			"Arial Cyr"/*lpszFacename*/);
	}

	bool in(int x,int y)
	{
		if (visible)
		{
		if(x>=left &&x<=left+width && y>=top && y<=top+height)
			return true;
		else return false;
		}
		else return false;
	}
	void paint(HDC Out)
	{		
		if (visible)
		{
			SetTextAlign(Out,TA_CENTER|TA_BASELINE);
			if(enabel)
				{
					SelectObject(Out,ecvetbord);
					SelectObject(Out,ecvetzal);
					SelectObject(Out,efont);
					SetBkColor(Out,ebkcolor);
					SetTextColor(Out,efontcolor);
				}
			else
			{
				SelectObject(Out,cvetbord);
				SelectObject(Out,cvetzal);
				SelectObject(Out,font);
				SetBkColor(Out,bkcolor);
				SetTextColor(Out,fontcolor);
			}
			Rectangle(Out,left,top,left+width,top+height);	
			TextOut(Out,int(left+width/2),int(top+height/2),name,(int)strlen(name));
			
			
			/*
			if (enabel)
			{
				if (width/2>=strlen(name)*fontsize/4.45)
				{
				TextOut(Out,int(left+width/2-strlen(name)*fontsize/4.45),int(top+height/2-fontsize/2 ),name,(int)strlen(name));
				}
				else
				{
					TextOut(Out,int(left+1),int(top+height/2-fontsize/2 ),name,(int)strlen(name));
				}
			}
			else
			{
				if (width/2>strlen(name)*fontsize/4.45)
				{
				TextOut(Out,int(left+width/2-strlen(name)*fontsize/4.45),int(top+height/2-fontsize/2 ),name,(int)strlen(name));
				}
				else
				{
					TextOut(Out,int(left+1),int(top+height/2-fontsize/2 ),name,(int)strlen(name));
				}
			}
			*/
		}


	}
};
class chackbox_c					//На основе	этой структуры рисуются	ChackBox`ы (все)
{
public :
	int	top;
	int	left;
	int	width;
	int	height;
	char name[25];	
	bool enabel;
	bool visible;
	HFONT font;
	/*HPEN cvetbord;
	HBRUSH cvetzal;*/
	COLORREF fon;
	bool in(int x,int y)
	{
		if(x>=left &&x<=left+width && y>=top && y<=top+height)
			return true;
		else return false;
	}
	void paint (HDC	Out)
	{
		if (visible)
		{
			HPEN pen;
			HBRUSH brush;
			pen=CreatePen(0,1,fon);
			brush=CreateSolidBrush(fon);
			SelectObject(Out,pen);
			SelectObject(Out,brush);
			Rectangle(Out,left,top,left+width,top+height);
			DeleteObject(pen);
			pen=CreatePen(0,1,RGB(0,0,0));
			SelectObject(Out,pen);
			Rectangle(Out,left+2,top+2,left+12,top+12);
			if (enabel)
			{
				MoveToEx(Out,left+2,top+2,NULL);
				LineTo(Out,left+12,top+12);
				MoveToEx(Out,left+12,top+2,NULL);
				LineTo(Out,left+2,top+12);
			};
			SetBkColor(Out,fon);
			SetTextColor(Out,RGB(0,0,0));
			SelectObject(Out,font);
			TextOut(Out,left+15,top,name,(int)strlen(name));
			DeleteObject(pen);
			DeleteObject(brush);
		}
	};
};
class combobox_c
{
	public :

	HWND hWndcom;
	int	top;
	int	left;
	int	width;
	int	height;
	char name[25];


	void add(char *strin)	//Добавляет строчку
	{
		char temp[255];
		strcpy(temp,strin);

		SendMessage(hWndcom,CB_ADDSTRING,0,	(LPARAM)temp);

	}
	void clear ()
	{
		SendMessage(hWndcom,CB_RESETCONTENT,0,0);
	};

	void setindex(int index)	//Выводит на позицию
	{
		SendMessage(hWndcom,CB_SETCURSEL,index,0);
	}
	
	int getindex()		//Возращеат номер позиции
	{
		int rez;
		rez = SendMessage(hWndcom,CB_GETCURSEL,0,0); 
		return rez;
	}

	int count() //Возращает число элементов
	{
		int rez;
		rez=SendMessage(hWndcom,CB_GETCOUNT,0,0); 
		return(rez);
	}

	void hide()
	{
		SendMessage(hWndcom,CB_SHOWDROPDOWN,FALSE,0);
		ShowWindow(hWndcom,SW_HIDE);
	}
	void show(bool drop)
	{
		ShowWindow(hWndcom,SW_SHOW);
		if (drop) SendMessage(hWndcom,CB_SHOWDROPDOWN,TRUE,0);
		else SendMessage(hWndcom,CB_SHOWDROPDOWN,FALSE,0);
	}
	BOOL move (int left_n,int top_n,int width_n,int height_n)
	{
		left=left_n;
		top=top_n;
		width=width_n;
		height=height_n;
		BOOL rez;

		rez = SetWindowPos(hWndcom,
		HWND_TOP,
		left_n,
		top_n,
		width_n,
		height_n,
		NULL);
		return rez;

	}
	
	void create(HWND hWnd)
	{
		DestroyWindow(hWndcom);			
				

		hWndcom= CreateWindow("COMBOBOX", name, 
                CBS_DROPDOWNLIST | WS_CHILD | WS_VISIBLE | WS_VSCROLL ,
               left, 
               top, 
               width, 
               height, 
                hWnd, NULL, NULL, NULL);
		

	}

};

class label_c
{
private:
	HFONT font;					//Используемы шрифт
	int fontsize;
	bool vert;
	int Orientation;
public:

	int	top;					//Позиция с верху
	int	left;					//позиция с лева
	int	width;					//Ширина
	int	height;					//Высота
	int textlong;				//Ширина надписи
	char name[255];				//Надпись
	bool visible;				//Видимость
	COLORREF fontcolor;			//Цвет надписи
	COLORREF bkcolor;			//Цвет под надписью
	int mark;					//Маркер (мало ли пригодиться)
	bool center;				//Центрировать


	void setfontsize(int size)
	{
		fontsize=size;
		font=CreateFont(fontsize/*nHeight*/,
				0/*nWidth*/,
				Orientation/*nEscapement*/,
				0/*nOrientation*/,
				FW_NORMAL/*nWeight*/,
				FALSE/*bItalic*/,
				FALSE/*bUnderline*/,
				0/*cStrikeOut*/,
				RUSSIAN_CHARSET/*nCharSet*/,
				OUT_DEFAULT_PRECIS/*nOutPrecision*/,
				CLIP_DEFAULT_PRECIS/*nClipPrecision*/,
				DEFAULT_QUALITY/*nQuality*/,
				DEFAULT_PITCH |	FF_SWISS/*nPitchAndFamily*/,
			"Arial Cyr"/*lpszFacename*/);
	}

	label_c()
	{
		strcpy(name,"Подпись");
		top=10;
		left=10;
		width=120;
		height=14;
		visible=true;
		bkcolor=RGB(0,0,0);
		fontcolor=RGB(100,250,100);
		Orientation=0;
		setfontsize(14);
		textlong=0;
		mark=0;
		center=false;
	}
	void setvertical(bool vol)
	{
		vert=vol;
		if (vert) 
		{
			Orientation=900;
		}
		else
		{
			Orientation=0;
		}
		setfontsize(fontsize);
	}
	
	bool in(int x,int y)
	{
		if (visible)
		{
			if(x>=left &&x<=left+width && y>=top && y<=top+height)
			{
				return true;
			}
			else return false;
		}
		else return false;
	}

	
	void settext(char *strin)	//Выводимый текст
	{
		strcpy(name,strin);
	}

	void paint(HDC Out) // Нарисовать окно
	{		
		if (visible)
		{
			HDC dc_buf;	//Это сами буфера
			HBITMAP bit_buf;
			HGDIOBJ pbit_buf;	//Это все для буферов
			HBRUSH brush;
			
			dc_buf=CreateCompatibleDC(Out);
			bit_buf=CreateCompatibleBitmap(Out,width,height);
			SelectObject(dc_buf,bit_buf);
			pbit_buf=SelectObject(dc_buf,bit_buf);
			if (center) SetTextAlign(dc_buf, TA_CENTER);
			SetBkColor(dc_buf,bkcolor);
			SelectObject(dc_buf,font);
			SetTextColor(dc_buf,fontcolor);

			brush=CreateSolidBrush(bkcolor);
			SelectObject(dc_buf,brush);
			Rectangle(dc_buf,-1,-1,width+1,height+1);
			DeleteObject(brush);

			SelectObject(dc_buf,font);
			if (vert==false)
			{
				if (center)
				{
					TextOut(dc_buf,int(width/2),0,name,strlen(name));
				}
					else
				{
					TextOut(dc_buf,0,0,name,strlen(name));
				}
			}
			else
			{
				if (center)
				{
					TextOut(dc_buf,0,height/2,name,strlen(name));
				}
					else
				{
					TextOut(dc_buf,0,height,name,strlen(name));
				}
				
			}
			BitBlt(Out,left,top,left+width,top+height,dc_buf,0,0,SRCCOPY);
			if (pbit_buf) SelectObject(dc_buf,pbit_buf);
			DeleteDC(dc_buf);
			DeleteObject(bit_buf);
		
		}
	}


};

class textedit_c
{
	public :

	HWND hWndControl;
	int	top;
	int	left;
	int	width;
	int	height;
	char name[25];

	void settext(char *string)	//Выводит на позицию
	{
		char temp[255];
		strcpy(temp,string);  
		SendMessage(hWndControl,WM_SETTEXT,0,(LPARAM)temp);  
	}
	
	int gettext(char *string)		//Возращеат текст
	{
		SendMessage(hWndControl,EM_GETLINE,0,*string);  
	}

	void hide()
	{
		ShowWindow(hWndControl,SW_HIDE);
	}
	void setcoun(int i)
	{
		SendMessage(hWndControl,EM_LIMITTEXT,i,0);
	};

	void setfocus()
	{
		SetFocus(hWndControl);
	}
	void show()
	{
		ShowWindow(hWndControl,SW_SHOW);
		SendMessage(hWndControl,EM_SETSEL,0,-1); 
		
	}

	double strtofloat(HWND hWnd, char *tmp)
	{
		int i,lon,count;
		double rez,kd;
		char ch;
		lon=strlen(tmp);
		bool det;
		count=0;
		det=true;
		for (i=0;i<lon;i++)
		{
			ch=tmp[i];
			if (ch>='0' && ch<='9')
			{} 
			else
			{
				if (ch=='-' && i==0)
				{}
				else
				{
					if (ch=='.'&& count==0)
					{count=1;}
					else
					{
						if(ch==',' && count==0)
						{count=1;}
						else
						{
							det=false; //Если ни одно из этого. то вводе числа была ошибка
						}
					}
				}

			}
		}

		if (det==false)
		{
			MessageBox(hWnd,"Ошибка ввота числа, операция прервана","Конвектор",MB_OK);
			return 0;
		}
		else
		{
			count=0;
			rez=0;
			kd=1;
			if (tmp[0]=='-') i=1;
			else i=0;
			for (;i<lon;i++)
			{
				if (tmp[i]!='.' && tmp[i]!=',')
				{
					if (count==0)
					{
						rez=rez*10+(int(tmp[i])-48);
					};
					if (count==1)
					{
						kd=kd/10;
						rez=rez+(int(tmp[i])-48)*kd;
					};
				}
				else count=1;

			}
			if (tmp[0]=='-') rez=-rez;

		}
		return rez;
	}

	double getfloat()
	{
		double rez;
		char temp[255];
		rez=0;
		SendMessage(hWndControl,WM_GETTEXT,255,(LPARAM)temp);
		rez=strtofloat(hWndControl,temp);
		rez=rez;
		return rez;
	}

	BOOL move (int left_n,int top_n,int width_n,int height_n)
	{
		left=left_n;
		top=top_n;
		width=width_n;
		height=16;
		BOOL rez;

		rez = SetWindowPos(hWndControl,
		HWND_TOP,
		left,
		top,
		width,
		height,
		NULL);
		return rez;

	}
	
	void create(HWND hWnd)
	{
		DestroyWindow(hWndControl);			
				

		hWndControl= CreateWindow("EDIT", name, 
                ES_AUTOHSCROLL | ES_LEFT | WS_CHILD | WS_VISIBLE | WS_BORDER ,
               left, 
               top, 
               width, 
               height, 
                hWnd, NULL, NULL, NULL);
		

	}

};


class series_c						//На основе	этой структуры рисуются	графики (все)
{
public:
	HDC dc_buf;	//Это сами буфера
	HBITMAP bit_buf;
	HGDIOBJ pbit_buf;	//Это все для буферов
	int	top;					//Позиция с верху
	int	left;					//позиция с лева
	int	width;					//Ширина
	int	height;					//Высота
	char name[25];				//Надпись
	bool visible;				//Видимость
	HPEN cvetbord;				//Кисть окантовки
	HBRUSH cvetzal;				//Кисть заливки
	HFONT font;					//Используемы шрифт
	COLORREF fontcolor;			//Цвет надписи
	COLORREF bkcolor;			//Цвет под надписью
	HPEN pen;					//Кисть, которой на ней рисуют
	HBRUSH brush;				//Кисть, которой ее закрашивают
	HDC *ptr_bitmap;			//Буфер, на который все будит сохраняться
	int bort;
	int lbort;

	struct  gr_point
	{
		double x;
		double y;
		gr_point *next;

	};
	gr_point *first_point;

private:


	
float step(float dist,int max_steps)
{
	int max_step;
	float kof;
	kof=0.001;
	max_step=max_steps;
	if (max_step<2) max_step=2;
	if (dist/0.001>=max_step) kof=0.001;
	if (dist/0.002>=max_step) kof=0.002;
	if (dist/0.005>=max_step) kof=0.005;
	if (dist/0.01>=max_step) kof=0.01;
	if (dist/0.02>=max_step) kof=0.02;
	if (dist/0.05>=max_step) kof=0.05;
	if (dist/0.1>=max_step) kof=0.1;
	if (dist/0.2>=max_step) kof=0.2;
	if (dist/0.5>=max_step) kof=0.5;
	if (dist/1>=max_step) kof=1;
	if (dist/2>=max_step) kof=2;
	if (dist/5>=max_step) kof=5;
	if (dist/10>=max_step) kof=10;
	if (dist/20>=max_step) kof=20;
	if (dist/50>=max_step) kof=50;
	if (dist/100>=max_step-1) kof=100;
	if (dist/200>=max_step-1) kof=200;
	if (dist/500>=max_step-1) kof=500;
	if (dist/1000>=max_step-1) kof=1000;
	if (dist/2000>=max_step-1) kof=2000;
	if (dist/5000>=max_step-1) kof=5000;
	if (dist/10000>=max_step-1) kof=10000;
	if (dist/20000>=max_step-1) kof=20000;
	if (dist/50000>=max_step-1) kof=50000;
	return kof;
}



	void XY_refresh()
	{
		double maxx,minx,maxy,miny;
		gr_point *tmp;
		tmp=first_point;
		maxx=0;
		minx=0;
		maxy=0;
		miny=0;


		if (tmp!=NULL)
		{
			maxx=tmp->x;
			minx=tmp->x;
			maxy=tmp->y;
			miny=tmp->y;
			if (tmp->next!=NULL)
			do 
			{
				tmp=tmp->next;
				maxx=__max(maxx,tmp->x);
				minx=__min(minx,tmp->x);
				maxy=__max(maxy,tmp->y);
				miny=__min(miny,tmp->y);				
			} while(tmp->next!=NULL);

		}
		if (maxx==minx)
		{
			maxx=maxx+1;
			minx=minx-1;
		}
		else
		{
			maxx=maxx+(maxx-minx)*0.02;
			minx=minx-(maxx-minx)*0.02;
		}

		if (maxy==miny)
		{
			maxy=maxy+1;
			miny=miny-1;
		}
		else
		{
			maxy=maxy+(maxy-miny)*0.02;
			miny=miny-(maxy-miny)*0.02;
		}

		HPEN nPen;
		HBRUSH nBrush;

		nPen=CreatePen(0,1,RGB(0,255,0));
		nBrush=CreateSolidBrush(RGB(0,255,0));

		SelectObject(dc_buf,cvetbord);
		SelectObject(dc_buf,cvetzal);
		Rectangle(dc_buf,0,0,width,height);

		initgraf(dc_buf);
		SelectObject(dc_buf,cvetzal);

		//Rectangle(dc_buf,bort,bort,width-lbort,height-bort);

		initgraf(dc_buf);
		SelectObject(dc_buf,nPen);
		SelectObject(dc_buf,nBrush);
		
		int xc,yc;
		if (first_point!=NULL)
		{

			

			if (first_point->next==NULL)
			{
				yc=height-(bort+int((height-2*bort)*(first_point->y-miny)/(maxy-miny)));
				xc=int(bort+(width-bort-lbort)*(first_point->x-minx)/(maxx-minx));
				Ellipse(dc_buf,xc-2,yc-2,xc+2,yc+2);
			}
			else
			{
				tmp=first_point;
			do {
				yc=height-(bort+int((height-2*bort)*(tmp->y-miny)/(maxy-miny)));
				xc=int(bort+(width-bort-lbort)*(tmp->x-minx)/(maxx-minx));
				MoveToEx(dc_buf,xc,yc,NULL);
				tmp=tmp->next;
				yc=height-(bort+int((height-2*bort)*(tmp->y-miny)/(maxy-miny)));
				xc=int(bort+(width-bort-lbort)*(tmp->x-minx)/(maxx-minx));
				LineTo(dc_buf,xc,yc);		

				}
			while (tmp->next !=NULL);
			}

		}








		//SelectObject(dc_buf,cvetbord);
		//SelectObject(dc_buf,cvetzal);
		initgraf(dc_buf);
				
		int i;
		float dist,kof;
		int coor;
		char temp[128];

			SetTextAlign(dc_buf,TA_LEFT);
			dist=float(maxy-miny);
			kof=step(dist,(height-2*bort)/(bort*2));	
			for (i=int(miny/kof);i<int(maxy/kof)+1;i++)
			{
				coor=height-(bort+int((height-2*bort)*(i*kof-miny)/(maxy-miny)));

				if (coor>bort && coor<height-bort)
				{
					
					MoveToEx(dc_buf,bort,coor,NULL);
					LineTo(dc_buf,width-lbort,coor);
					sprintf(temp," %g ",i*kof);	
					TextOut(dc_buf,width-lbort*0.9,coor-8,temp,int(strlen(temp)));
				}
			};
		
			SetTextAlign(dc_buf,TA_CENTER);
			dist=float(maxx-minx);
			kof=step(dist,(width-bort-lbort)/(lbort*2));	
			for (i=int(minx/kof);i<int(maxx/kof)+1;i++)
			{
				coor=bort+int((width-bort-lbort)*(i*kof-minx)/(maxx-minx));

				if (coor>bort && coor<width-lbort)
				{
					
					MoveToEx(dc_buf,coor,bort,NULL);
					LineTo(dc_buf,coor,height-bort);
					sprintf(temp," %g ",i*kof);
					TextOut(dc_buf,coor,height-bort,temp,int(strlen(temp)));
				}
			};


















		DeleteObject(nPen);
		DeleteObject(nBrush);
	}

	void del_point(gr_point *tmp)
		{
			if (tmp!=NULL)
			{			
				if (tmp->next != NULL)
					del_point(tmp->next);
				delete (tmp);
				int i;
				i=0;
			}
		}
	
	
public:
	series_c ()
	{
		strcpy(name, "График");
		visible=true;
		bkcolor=RGB(0,0,0);
		cvetbord=CreatePen(0,1,bkcolor);
		cvetzal=CreateSolidBrush(bkcolor);
		pen=CreatePen(0,1,RGB(100,100,100));
		brush=CreateSolidBrush(RGB(100,100,100));
		fontcolor=RGB(20,170,20);
		font=CreateFont(14/*nHeight*/,
			0/*nWidth*/,
			0/*nEscapement*/,
			0/*nOrientation*/,
			FW_NORMAL/*nWeight*/,
			FALSE/*bItalic*/,
			FALSE/*bUnderline*/,
			0/*cStrikeOut*/,
			RUSSIAN_CHARSET/*nCharSet*/,
			OUT_DEFAULT_PRECIS/*nOutPrecision*/,
			CLIP_DEFAULT_PRECIS/*nClipPrecision*/,
			DEFAULT_QUALITY/*nQuality*/,
			DEFAULT_PITCH |	FF_SWISS/*nPitchAndFamily*/,
			"Arial Cyr"/*lpszFacename*/);
		left=10;
		top=10;
		width=200;
		height=100;
		first_point=NULL;
		bort=15;
		lbort=50;
	}
	bool in(int x,int y)
	{
		if(x>=left &&x<=left+width && y>=top && y<=top+height)
			return true;
		else return false;
	}

	void initgraf(HDC Out) //Инициалезирует набор кистей для рисования на Out
	{
		SelectObject(Out,pen);
		SelectObject(Out,brush);
		SetBkColor(Out,bkcolor);
	}
	

	void init_dc(HWND hWnd) //Переразметка буфера окна
	{
		DeleteDC(dc_buf);											//Удаление буфера
		DeleteObject(bit_buf);
		dc_buf=CreateCompatibleDC(GetDC(hWnd));
		bit_buf=CreateCompatibleBitmap(GetDC(hWnd),width,height);
		SelectObject(dc_buf,bit_buf);
		pbit_buf=SelectObject(dc_buf,bit_buf);
		RECT winr;
		winr.left=0;
		winr.top=0;
		winr.bottom=height;
		winr.right=width;

		FillRect(dc_buf,&winr,cvetzal);
		ptr_bitmap=&dc_buf;
	}

	void XY_add(double x, double y)
	{
		gr_point *po;
		po = new gr_point;
		po->x=x;
		po->y=y;
		po->next=NULL;
		if (first_point == NULL)
		{
			first_point=new gr_point;
			first_point=po;
		}
		else
		{
			gr_point *temp;
			temp=first_point;
			while (temp->next!=NULL) 
			{
				temp=temp->next;
			}
			temp->next=po;
		}
	}

  
	void XY_clear()
	{		
		del_point(first_point);
		first_point=NULL;
	}

	
	void paint(HDC Out) // Нарисовать окно
	{		
		if (visible)
		{
			XY_refresh();
			initgraf(dc_buf);
			SetBkMode(dc_buf, TRANSPARENT);
			/*double maxx,minx,maxy,miny;
			gr_point *tmp;
			tmp=first_point;
			maxx=0;
			minx=0;
			maxy=0;
			miny=0;
			if (tmp!=NULL)
			{
				maxx=tmp->x;
				minx=tmp->x;
				maxy=tmp->y;
				miny=tmp->y;
				if (tmp->next!=NULL)
					do 
					{
						tmp=tmp->next;
						maxx=__max(maxx,tmp->x);
						minx=__min(minx,tmp->x);
						maxy=__max(maxy,tmp->y);
						miny=__min(miny,tmp->y);				
					} while(tmp->next!=NULL);
					
			}
			if (maxx==minx)
			{
				maxx=maxx+1;
				minx=minx-1;
			}
			else
			{
				maxx=maxx+(maxx-minx)*0.02;
				minx=minx-(maxx-minx)*0.02;
			}
			
			if (maxy==miny)
			{
				maxy=maxy+1;
				miny=miny-1;
			}
			else
			{
				maxy=maxy+(maxy-miny)*0.02;
				miny=miny-(maxy-miny)*0.02;
			}

			int i,kof,coor;
			float dist;
			char temp[40];
			SetTextAlign(dc_buf,TA_RIGHT);
			dist=float(maxy-miny);
			kof=step(dist,(height-2*bort)/(lbort*2));	
			for (i=int(miny/kof);i<int(maxy/kof)+1;i++)
			{
				coor=height-(bort+int((height-2*bort)*(i*kof-miny)/(maxy-miny)));

				if (coor>bort && coor<height-bort)
				{
					
					MoveToEx(dc_buf,bort,coor,NULL);
					LineTo(dc_buf,width-lbort,coor);
					sprintf(temp," %d ",i*kof);	
					TextOut(dc_buf,width,coor-8,temp,int(strlen(temp)));
				}
			};
		
			SetTextAlign(dc_buf,TA_CENTER);
			dist=float(maxx-minx);
			kof=step(dist,(width-bort-lbort)/(lbort*5));	
			for (i=int(minx/kof);i<int(maxx/kof)+1;i++)
			{
				coor=bort+int((width-bort-lbort)*(i*kof-minx)/(maxx-minx));

				if (coor>bort && coor<width-lbort)
				{
					
					MoveToEx(dc_buf,coor,bort,NULL);
					LineTo(dc_buf,coor,height-bort);
					sprintf(temp," %d ",i*kof);
					TextOut(dc_buf,coor,height-bort,temp,int(strlen(temp)));
				}
			};
			*/
			SetTextAlign(dc_buf,TA_LEFT);
			SetBkColor(dc_buf,bkcolor);
			
			SelectObject(dc_buf,font);
			SetTextColor(dc_buf,fontcolor);
			TextOut(dc_buf,5,5,name,int(strlen(name)));
			BitBlt(Out,left,top,left+width,top+height,dc_buf,0,0,SRCCOPY);
		}
	}
		
	
};

