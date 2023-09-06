// C++ code
//
int num_frame[8][50]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                      {0,1,1,0,0,0,1,1,0,0,0,1,1,1,0,1,1,1,1,0,0,0,0,1,0,1,1,1,1,0,0,0,1,0,0,1,1,1,1,1,0,0,1,1,0,0,0,1,1,1},
                      {1,0,0,1,0,1,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,1,0,0,1,0,1,0,0,1},
                      {1,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,1,1,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,0,1,0,0,1,1,1},
                      {1,0,0,1,0,0,0,1,0,0,0,0,1,0,0,0,1,0,0,0,1,1,1,1,0,0,0,1,0,0,1,1,1,0,0,0,1,1,1,1,0,0,1,1,0,0,0,0,0,1},
                      {1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1,0},
                      {0,1,1,0,0,0,1,1,1,0,1,1,1,1,0,0,0,0,1,0,0,0,0,1,0,1,1,0,0,0,1,0,0,1,0,0,1,0,0,0,0,1,0,0,1,0,0,1,0,0},
                      {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,0,0,0,0,0,0,0,1,1,0,0,1,0,0,0,0,0,0,1,1,0,0,1,0,0,0}};
                      
int frame[8][16]={{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0},
                  {0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0},
                  {0,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0},
                  {0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
                  {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}};
//bool finish=false;
struct Vec2
{
  int x;
  int y;
};
enum dir
{
  up,
  down,
  left,
  right
};
class rectangle
{
  public:
  Vec2 pos;
  dir my_dir;
};
rectangle snake[128];
rectangle frog;
int length=4;
int score=0;
bool restart=false;
int restart_timer=0;
void setup()
{
   pinMode(0, OUTPUT);
   pinMode(1, OUTPUT);
   pinMode(2, OUTPUT);
   pinMode(3, OUTPUT);
   pinMode(4,OUTPUT);
   pinMode(13, OUTPUT);
   pinMode(A0,INPUT);
   pinMode(A1,INPUT);
  initialize();
}


bool collision_det()
{
  for(int i=1;i<length;i++)
  {
    if(snake[0].pos.x==snake[i].pos.x && snake[0].pos.y==snake[i].pos.y)
    {
      return true;
    }
  }
  return false;
}


void loop()
{
  if(!restart)
  {
  update();
  draw_call();
  render();
  }
  else
  {
    if(restart_timer<20)
    {
      restart_timer++;
      int temp_score=score;
      int dig[3]={0,0,0};
      int i=2;
      while(i>=0)
      {
        dig[i]=temp_score%10;
        temp_score/=10;
        i--;
      }
      // draw score output.........
      for(int i=0;i<8;i++)
      for(int j=0;j<16;j++)
      frame[i][j]=0;
      
      for(int j=0;j<3;j++)//3 digits
      {
        for(int k=0;k<8;k++)//8 rows each digit
        {
          int strt=dig[j]*5;
         
          for(int l=strt;l<strt+5;l++)//5 collumn for each digit from strt
          {
            if(num_frame[k][l]>0)
            {
              frame[7-k][(j*5)+l-strt]=1;
            }
          }
         }
      }      
      //call renderer  for score
      render();
    }
    else
    {
      restart_timer=0;
      restart=false;
      initialize();
    }
  }
  
  
}


void initialize()
{
  
  //initialize all call once 
  frog.pos={6,9};//frog_gen 
  frog.my_dir=up;
  length=4;
  score=0;
  snake[0].pos={3,5};
  snake[0].my_dir=right;
  for(int i=1;i<128;i++)
  {
    snake[i].pos={snake[i-1].pos.x,(snake[i-1].pos.y-1)};
    snake[i].my_dir=right;
  }
}


struct Vec2 input_handler()
{
  Vec2 inp;
  inp.x=0;
  inp.y=0;
  float joystic_ud=((analogRead(A0)/1023.0)*2)-1;
  float joystic_lr=((analogRead(A1)/1023.0)*2)-1;
  //setting x
  if(joystic_ud>0.5)
  {
    inp.x=1;
  }
  else if(joystic_ud<-0.5)
  {
  inp.x=-1;
  }
  else
  inp.x=0;
  //setting y
  if(joystic_lr>0.5)
  {
    inp.y=1;
  }
  else if(joystic_lr<-0.5)
  {
  inp.y=-1;
  }
  else
  inp.y=0;
  return inp;
}


void frog_gen(int fail)
{
  frog.pos.x=random(1,6);
  frog.pos.y=random(1,14);
     for(int i=0;i<length;i++)
      {
      if(snake[i].pos.x==frog.pos.x && snake[i].pos.y==frog.pos.y)
        frog_gen(1);
      }
}

void update()
{
  for(int i=127;i>0;i--)
  {
    snake[i].pos=snake[i-1].pos;
    snake[i].my_dir=snake[i-1].my_dir;
  }
  Vec2 inx={0,0};
  inx=input_handler();
  //x and y cant be of absolute max value at once 
    if(inx.x==0)
    {
      if(inx.y==1)
      {
        if(snake[0].my_dir!=left)
        {
          snake[0].my_dir=right;
          snake[0].pos.y+=1;
        }
        else
        {
          snake[0].pos.y-=1;
        }
        
      }
      else if(inx.y==-1)
      {
        if(snake[0].my_dir!=right)
        {
          snake[0].my_dir=left;
          snake[0].pos.y-=1;
        }
        else
        {
           snake[0].pos.y+=1;
        }
        
      }
      else if(inx.y==0)
      {
         switch(snake[0].my_dir)
         {
           case up:
        snake[0].pos.x-=1;
            break;
           case down:
            snake[0].pos.x+=1;
            break;
           case left:
            snake[0].pos.y-=1;
            break;
           case right:
            snake[0].pos.y+=1;
            break;
         }
       }
    }
  else if(inx.x==1)
  {
    if(snake[0].my_dir!=up)
    {
      snake[0].my_dir=down;
      snake[0].pos.x+=1;
    }
    else
    {
       snake[0].pos.x-=1;
    }
    
  }
  else if(inx.x==-1)
  {
    if(snake[0].my_dir!=down)
    {
      snake[0].my_dir=up;
      snake[0].pos.x-=1;
    }
    else
    {
      snake[0].pos.x+=1;
    }
    
  }
  //wrap
  if(snake[0].pos.x>7)
  {
    snake[0].pos.x=0;
  }
  if(snake[0].pos.x<0)
  {
    snake[0].pos.x=7;
  }
  if(snake[0].pos.y>15)
  {
    snake[0].pos.y=0;
  }
  if(snake[0].pos.y<0)
  {
    snake[0].pos.y=15;
  }
  //eat
  if(snake[0].pos.x==frog.pos.x && snake[0].pos.y==frog.pos.y)
  {
    length++;
    score++;
    if(length>127)
    {
      length=127;
    }
    frog_gen(0);
  }
  if(collision_det())
  {
    restart=true;
    
   // initialize();
  }
  
}

void draw_call()
{
  for(int i=0;i<8;i++)
    for(int j=0;j<16;j++)
      frame[i][j]=0;
    
  frame[frog.pos.x][frog.pos.y]=1;
  for(int i=0;i<length;i++)
    frame[snake[i].pos.x][snake[i].pos.y]=1;
}

void render()
{
   digitalWrite(0, HIGH);
  digitalWrite(1, HIGH);//big clk pulse for next row selection
  digitalWrite(1, LOW);//...
 digitalWrite(4, HIGH);//big clk pulse for latching row selection
 digitalWrite(4, LOW);//...
  for(int i=0;i<8;i++)
  {
    for(int j=0;j<16;j++)
    {
      if(frame[i][15-j]==1)
      {
        digitalWrite(2, HIGH);
      }
      else
      {
         digitalWrite(2, LOW);
      }
      //input in collumns
      digitalWrite(3, HIGH);//small clk pulse for next collumn selection
      digitalWrite(3, LOW);//...
     
    }
    delay(10);
    digitalWrite(0, LOW);
    digitalWrite(1, HIGH);//big clk pulse for next row selection
    digitalWrite(1, LOW);//...
   digitalWrite(4, HIGH);//big clk pulse for latching row selection
    digitalWrite(4, LOW);//...
   }
}
