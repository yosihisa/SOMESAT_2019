#include <SD.h>
 
const uint8_t cs_SD = 4;
 
const char* fname1 ="/test1.txt";
const char* fname2 ="/test2.txt";
const char* fname3 ="/test3.txt";
const char* fname4 ="/test4.txt";
const char* fname5 ="/test5.txt";
const char* fname6 ="/test6.txt";
const char* fname7 ="/test7.txt";
const char* fname8 ="/test8.txt";
const char* fname9 ="/test9.txt";
const char* fname10 ="/test10.txt";
 
File f1, f2, f3, f4, f5, f6, f7, f8, f9, f10;
 
void setup() {
  Serial.begin(115200);
  SD.begin(cs_SD, SPI, 24000000, "/sd");
 
  Serial.println("\r\n--------正常な書き込み（ファイルは４つまで同時オープン）");
  f1 = SD.open(fname1, FILE_WRITE);
  f2 = SD.open(fname2, FILE_WRITE);
  f3 = SD.open(fname3, FILE_WRITE);
  f4 = SD.open(fname4, FILE_WRITE);
  serial_print_Fwrite(f1, 1, "このファイルはテスト1\r\n");
  serial_print_Fwrite(f2, 2, "このファイルはテスト2\r\n");
  serial_print_Fwrite(f3, 3, "このファイルはテスト3\r\n");
  serial_print_Fwrite(f4, 4, "このファイルはテスト4\r\n");
  f1.close(); f2.close(); f3.close(); f4.close();
  delay(1);
   
  f5 = SD.open(fname5, FILE_WRITE);
  f6 = SD.open(fname6, FILE_WRITE);
  f7 = SD.open(fname7, FILE_WRITE);
  f8 = SD.open(fname8, FILE_WRITE);
  serial_print_Fwrite(f5, 5, "このファイルはテスト5\r\n");
  serial_print_Fwrite(f6, 6, "このファイルはテスト6\r\n");
  serial_print_Fwrite(f7, 7, "このファイルはテスト7\r\n");
  serial_print_Fwrite(f8, 8, "このファイルはテスト8\r\n");
  f5.close(); f6.close(); f7.close(); f8.close();
  delay(1);
   
  f9 = SD.open(fname9, FILE_WRITE);
  f10 = SD.open(fname10, FILE_WRITE);
  serial_print_Fwrite(f9, 9, "このファイルはテスト9\r\n");
  serial_print_Fwrite(f10, 10, "このファイルはテスト10\r\n");
  f9.close();  f10.close();
  Serial.println("\r\n--------10ファイル読み込み同時オープン");
   
   Serial.println("");
  f1 = SD.open(fname1, FILE_READ);
  f2 = SD.open(fname2, FILE_READ);
  f3 = SD.open(fname3, FILE_READ);
  f4 = SD.open(fname4, FILE_READ);
  f5 = SD.open(fname5, FILE_READ);
  f6 = SD.open(fname6, FILE_READ);
  f7 = SD.open(fname7, FILE_READ);
  f8 = SD.open(fname8, FILE_READ);
  f9 = SD.open(fname9, FILE_READ);
  f10 = SD.open(fname10, FILE_READ);
 
  serial_print_Fread(f1, 1);
  serial_print_Fread(f2, 2);
  serial_print_Fread(f3, 3);
  serial_print_Fread(f4, 4);
  serial_print_Fread(f5, 5);
  serial_print_Fread(f6, 6);
  serial_print_Fread(f7, 7);
  serial_print_Fread(f8, 8);
  serial_print_Fread(f9, 9);
  serial_print_Fread(f10, 10);
 
  f1.close(); f2.close(); f3.close(); f4.close();  f5.close();
  f6.close(); f7.close(); f8.close(); f9.close();  f10.close();
   
  Serial.println("\r\n---------ファイルを４つ毎に読み込む");
  f1 = SD.open(fname1, FILE_READ);
  f2 = SD.open(fname2, FILE_READ);
  f3 = SD.open(fname3, FILE_READ);
  f4 = SD.open(fname4, FILE_READ);
  serial_print_Fread(f1, 1);
  serial_print_Fread(f2, 2);
  serial_print_Fread(f3, 3);
  serial_print_Fread(f4, 4);  
  f1.close(); f2.close(); f3.close(); f4.close();
  delay(1);
   
  f5 = SD.open(fname5, FILE_READ);
  f6 = SD.open(fname6, FILE_READ);
  f7 = SD.open(fname7, FILE_READ);
  f8 = SD.open(fname8, FILE_READ);
  serial_print_Fread(f5, 5);
  serial_print_Fread(f6, 6);
  serial_print_Fread(f7, 7);
  serial_print_Fread(f8, 8);
  f5.close(); f6.close(); f7.close(); f8.close();
  delay(1);
 
  f9 = SD.open(fname9, FILE_READ);
  f10 = SD.open(fname10, FILE_READ);  
  serial_print_Fread(f9, 9);
  serial_print_Fread(f10, 10);
  f9.close(); f10.close();
/*  
  Serial.println("\r\n------------ファイル削除");
  Delete_File(fname1);
  Delete_File(fname2);
  Delete_File(fname3);
  Delete_File(fname4);
  Delete_File(fname5);
  Delete_File(fname6);
  Delete_File(fname7);
  Delete_File(fname8);
  Delete_File(fname9);
  Delete_File(fname10);
*/
}
 
void serial_print_Fwrite(File ff, byte num, const char *message){
  if(ff.print(message)){
    Serial.printf("f%d Message wrote. Good!\r\n", num);
  }else{
    Serial.printf("f%d write failed\r\n", num);
  }
}
 
void serial_print_Fread(File ff, byte num){
  size_t len = ff.size();
  uint8_t buf[256];
   
  if (ff.read(buf, len)) {
    Serial.printf("f%d File found OK!!\r\n", num);
    for(int i=0; i<len; i++){
      Serial.write(buf[i]);
    }
  }else{
    Serial.printf("f%d File not found\r\n", num);
  }
}
 
void Delete_File(const char *fname){
  if(SD.remove(fname)){
      Serial.printf("%s File deleted\r\n", fname);
  } else {
      Serial.printf("%s Delete failed\r\n", fname);
  }
}
 
void loop() {
}
