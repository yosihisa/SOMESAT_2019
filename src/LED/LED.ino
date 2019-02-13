const int led4 = 18;
const int led5 = 19;


void setup() {
  pinMode( led4, OUTPUT );
  pinMode( led5, OUTPUT );
}

void loop() {
  digitalWrite( led4, HIGH );
  digitalWrite( led5, HIGH );
  delay( 500 );
  digitalWrite( led4, LOW );
  digitalWrite( led5, LOW );
  delay( 500 );
}
