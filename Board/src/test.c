#define LINEBREADTH    10    //�߿�
#define LINECONCAT     8     //�ϲ�������� �ϲ�����ַ���
void AccommodFondLine(s8 *PixelAryy ,u8 PixelCount , s16 *LastLeftPixelStation,s16 *LastRingtPixelStation,u8 FAVAULE)
{//����
  static u8 NOLeftCount,NORingtCout ;//����
  s16 temp0B ,temp1B,temp2B,temp3B;//��ʱ
  u8 *LineStation ,LineCount ,*LineLeftStation,*LineRingtStation;
  s16 LeftMIN,LeftMAX,RingtMIN,RingtMAX;
  LineCount = 0 ;
  for(temp0B = 0 ; temp0B < PixelCount ; temp0B ++)//�ӵ�һ����ʼ����
  {
    temp1B = temp0B ;
    temp2B = 0 ;
    /***********
    ������߰���
    ***********/
    while(temp2B <= LINEBREADTH)//С���߿�
     {
      temp1B -- ;
      if(temp1B < 0)//С��0 ����
        break ;
      if( PixelAryy[temp1B] -  PixelAryy[temp0B] > FAVAULE )
      { temp2B ++ ;}
      else if(temp2B)
      { break ; }

    }

    /***********
    �����ұ߰���
    ***********/
    temp1B = temp0B ;
    temp3B = 0 ;
    while(temp3B <= LINEBREADTH)
    {
      temp1B ++ ;
      if(temp1B > PixelCount)
      { break ; }
      if( PixelAryy[temp1B] -  PixelAryy[temp0B] > FAVAULE )
      { temp3B ++ ;}
      else if(temp3B)
      { break ; }
    }
    /***********
    ��¼����λ��
    ***********/    
    if(temp2B >= LINEBREADTH ){
      *LineStation = temp0B ;
      LineCount ++ ;
    }else if(temp3B >= LINEBREADTH ){
      *LineStation = temp0B ;
      LineCount ++ ;
    }


  }
  /**********
  ���������Բ������Һ���λ��
  **********/
  if(LineCount)
  {
    temp2B = PixelCount >> 1 ;
    temp1B = NOLeftCount << 1;
    temp1B += LINECONCAT;
    LeftMIN = *LastLeftPixelStation - temp1B ;
    LeftMAX = *LastRingtPixelStation + temp1B ;
    if(LeftMIN < 0)
      LeftMIN = 0 ;

    if(LeftMAX > (temp2B + 1))
      LeftMAX  = temp2B + 1 ;

    RingtMIN = *LastRingtPixelStation - temp1B ;
    RingtMAX = *LastRingtPixelStation + temp1B ;
    if(RingtMAX > PixelCount)
       RingtMAX = PixelCount ;
    if(RingtMIN < (temp2B - 1)){
       RingtMIN = temp2B - 1 ;
    }
    temp2B = 0 ;
    temp3B = 0 ;
   for(temp1B = 0 ;temp1B < LineCount ;temp1B ++ )
   {
     if( (LeftMIN < LineStation[temp1B])&&(LineStation[temp1B]<LeftMAX))
     {
       LineLeftStation[temp2B] = LineStation[temp1B] ;
       temp2B ++ ;
     }else if( (RingtMIN < LineStation[temp1B])&&(LineStation[temp1B]<RingtMAX))
     {
       LineRingtStation[temp3B] = LineStation[temp1B] ;
       temp3B ++ ;
     }

   }

  }else 
  {
    NOLeftCount ++ ;
    NORingtCout ++ ;
  }

  if(temp2B)
  {
    NOLeftCount = 0 ;

  }

}