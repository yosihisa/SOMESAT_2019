
int[] data_max = {0, 0, 0, 0, 0, 0, 0};
Table[] data;

int[] log_num = {0, 0, 0, 0, 0, 0};

float latitude =33.0;
double m_per_deg = 10000.0*1000/90*cos(radians(latitude));

void setup() {
  size(1520, 800);
  PFont font;
  font = createFont("HGPｺﾞｼｯｸM", 18, true);
  textFont(font, 18);
  data = new Table[6];
  data[0] = loadTable("1.CSV", "header");
  data[1] = loadTable("2.CSV", "header");
  data[2] = loadTable("3.CSV", "header");
  data[3] = loadTable("4.CSV", "header");
  data[4] = loadTable("5.CSV", "header");
  data[5] = loadTable("6.CSV", "header");
  /*
  println(data_1.getRowCount());  // 行を数える
   println(data_1.getColumnCount()); // 列を数える
   println(data_1.getString(0, 0));  // 文字列呼び出し
   println(data_1.getString(2, 0));
   println(data_1.getFloat(0, 1));  // float型 呼び出し
   println(data_1.getFloat(2, 1));
   */
  data_max[0] =data[0].getRowCount();
  log_num[0] = 1;

  data_max[1] =data[1].getRowCount();
  log_num[1] = 1;

  data_max[2] =data[2].getRowCount();
  log_num[2] = 1;

  data_max[3] =data[3].getRowCount();
  log_num[3] = 1;
  data_max[4] =data[4].getRowCount();
  log_num[4] = 1;
  data_max[5] =data[5].getRowCount();
  log_num[5] = 1;
}
void draw() {

  int x_offset = 0;
  int y_offset = 0;
  int cx, cy, s;
  background(0, 0, 0);
  strokeWeight(0) ;
  for (int i =0; i<6; i++) {
    x_offset = 510*(i%3);
    if (i<3)y_offset=0;
    else y_offset =400;

    //カメラ画像の表示
    textAlign(RIGHT) ;
    fill(255, 255, 255, 255);
    if (data[i].getInt(log_num[i], 1) <2 && i <3) {
      PImage photo = loadImage(i+1 +"\\" + data[i].getString(log_num[i], 18) + ".JPG");
      pushMatrix(); 
      scale(-1, -1);
      image(photo, -180-x_offset, -10-y_offset, -320, -240);
      popMatrix();
      cx = data[i].getInt(log_num[i], 19);
      cy = data[i].getInt(log_num[i], 20);
      s  = data[i].getInt(log_num[i], 21);
      if (s >100) {
        text("("+cx+"," +cy+")", 180+x_offset, 25+22*7 + y_offset);
        text(s, 180+x_offset, 25+22*8 + y_offset);
        fill(0, 255, 255, 90);
        ellipse(180 + 320 +x_offset - cx, 10 + 240 +y_offset - cy, 16, 16);
      } else {
        text("(-,-)", 180+x_offset, 25+22*7 + y_offset);
        text(s, 180+x_offset, 25+22*8 + y_offset);
      }
    } else {
      fill(255, 255, 255, 255);
      rect(180+x_offset, 10+y_offset, 320, 240);
    }

    //各種ステータス
    textAlign(LEFT) ;
    fill(255, 255, 255, 255);
    text("時刻(UTC)", 0+x_offset, 25+22*0 + y_offset);  
    text("電圧", 0+x_offset, 25+22*1 + y_offset);  
    text("緯度  N", 0+x_offset, 25+22*2 + y_offset);
    text("経度  E", 0+x_offset, 25+22*3 + y_offset);
    text("状態 :", 0+x_offset, 25+22*4 + y_offset);
    text("距離", 0+x_offset, 25+22*5 + y_offset);
    text("距離(超音波)", 0+x_offset, 25+22*6 + y_offset);
    text("重心(画像)", 0+x_offset, 25+22*7 + y_offset);
    text("面積(画像)", 0+x_offset, 25+22*8 + y_offset);

    textAlign(RIGHT) ;
    text(data[i].getString(log_num[i], 2) + ":"+ data[i].getString(log_num[i], 3) + ":"+data[i].getString(log_num[i], 4), 180+x_offset, 25+22*0 + y_offset);
    text(data[i].getString(log_num[i], 8) + "V", 180+x_offset, 25+22*1 + y_offset);
    text(data[i].getString(log_num[i], 5), 180+x_offset, 25+22*2 + y_offset);
    text(data[i].getString(log_num[i], 6), 180+x_offset, 25+22*3 + y_offset);
    if (data[i].getInt(log_num[i], 1) == 0)text("GPS誘導中", 180+x_offset, 25+22*4 + y_offset);
    if (data[i].getInt(log_num[i], 1) == 1)text("最終誘導中", 180+x_offset, 25+22*4 + y_offset);
    if (data[i].getInt(log_num[i], 1) == 2)text("落下中", 180+x_offset, 25+22*4 + y_offset);
    if (data[i].getInt(log_num[i], 1) == 3)text("待機中", 180+x_offset, 25+22*4 + y_offset);

    //距離
    /*String dist_s =data[i].getString(log_num[i], 12);
     if(dist_s.length()>2){
     double dist =Double.parseDouble(dist_s);
     
     if(i < 3){
     dist = Math.sqrt(dist) * m_per_deg;
     }
     
     //  text(nfc(dist,3) + "m", 180+x_offset, 25+22*5 + y_offset);
     }
     else text("-m", 180+x_offset, 25+22*5 + y_offset);//距離
     */
    text(data[i].getString(log_num[i], 12), 180+x_offset, 25+22*5 + y_offset);//距離
    text(data[i].getString(log_num[i], 17), 180+x_offset, 25+22*6 + y_offset);

    //方位表示
    textAlign(LEFT) ;
    text("機体の向き[deg]", x_offset, 270 + y_offset);
    text("ｺﾞｰﾙ方向(GPS上)", x_offset, 290 + y_offset);  
    text("方位", x_offset, 310 + y_offset);
    fill(100, 100, 255, 255);
    rect(320-185 + x_offset, 290-18+y_offset, 365, 20);
    textAlign(CENTER);
    if (data[i].getInt(log_num[i], 1) <2) {
      int a = (int( 360 * (data[i].getFloat(log_num[i], 11)/(3.14*2))) +360)%360 ; //機体の方向
      int b = int( 360 * (data[i].getFloat(log_num[i], 13)/(3.14*2))) ;//機体から見たゴールの方向
      int c = (int( 360 * (data[i].getFloat(log_num[i], 14)/(3.14*2))) +360)%360 ;//GPS上でのゴール方向
      fill(255, 255, 255, 255);
      text(a, 320+x_offset, 270+y_offset);
      fill(255, 0, 0, 255);
      rect(320-20+ x_offset -b, 290-18+y_offset, 40, 20);
      fill(255, 255, 255, 255);
      text(c, 320-b+x_offset, 290+y_offset);

      //コンパス
      fill(255, 255, 255, 255);
      int d;
      d = 320-a;
      if (d<320-180)d +=360;
      if (d>320+180)d -=360;
      text("N", d+x_offset, 310+y_offset);
      d = 320-a + 90;
      if (d<320-180)d +=360;
      if (d>320+180)d -=360;
      text("E", d+x_offset, 310+y_offset);
      d = 320-a +180;
      if (d<320-180)d +=360;
      if (d>320+180)d -=360;
      text("S", d+x_offset, 310+y_offset);
      d = 320-a +270;
      if (d<320-180)d +=360;
      if (d>320+180)d -=360;
      text("W", d+x_offset, 310+y_offset);
    }

    fill(100, 255, 100, 128);
    rect(320-2 + x_offset, 290-40+y_offset, 4, 60);

    //モーター速度
    fill(255, 255, 255, 255);
    textAlign(CENTER);
    text("左　モータ速度　右", 320+x_offset, 360+y_offset);
    textAlign(RIGHT);
    int motorL, motorR;
    motorL = data[i].getInt(log_num[i], 15);
    motorR = data[i].getInt(log_num[i], 16);
    if (i<3) {
      motorL = 100 * motorL/1024;
      motorR = 100 * motorR/1024;
    } else {
      motorL = 100 * motorL/255;
      motorR = 100 * motorR/255;
    }
    text(motorL +"%", 230+x_offset, 360+y_offset);
    text(motorR +"%", 460+x_offset, 360+y_offset);

    textAlign(LEFT);
    text(i+1 +"号機", 0+x_offset, 360+y_offset);

    fill(255, 255, 255, 128);
    rect(500+ x_offset, y_offset, 5, 400);

    fill(255, 255, 255, 128);
    rect(0, 400, 1520, 5);
    if (log_num[i] < data_max[i]-1) {
      log_num[i]++;
    }
  }
  delay(200);
}