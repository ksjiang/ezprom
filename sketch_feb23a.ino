#define SDA 4   //serial data
#define SCL 3   //serial clock
#define SLA 2   //latch
#define RW 13    //memory R / W*

#define D0 5    //data bus
#define D1 6
#define D2 7
#define D3 8
#define D4 9
#define D5 10
#define D6 11
#define D7 12

void dispBytes(int, byte*, int);
void memByteStreamRead(int, int, int);
byte memByteRead(int);
void binaryCtr(int);
void sendByteStream(byte*);
void sendByte(byte);
byte readByte();
void pulseClock(int);

void setup() {
  pinMode(SDA, OUTPUT);
  pinMode(SCL, OUTPUT);
  pinMode(SLA, OUTPUT);

  pinMode(D0, INPUT);
  pinMode(D1, INPUT);
  pinMode(D2, INPUT);
  pinMode(D3, INPUT);
  pinMode(D4, INPUT);
  pinMode(D5, INPUT);
  pinMode(D6, INPUT);
  pinMode(D7, INPUT);

  dispBytes(10, 0, 256);
  return;
}

void loop() {
  return;
}

void dispBytes(int na, int startAddress, int numBytes) {
  byte m[numBytes];
  memByteStreamRead(na, startAddress, m, numBytes);
  Serial.begin(9600);
  Serial.println("----------------------MEMORY DUMP----------------------");
  int fulllines = numBytes / 16;
  int left = numBytes % 16;
  char line[80];
  for (int i = 0; i < fulllines; i++) {
    int base = startAddress + 16 * i;
    sprintf(line, "%04x: %02x %02x %02x %02x %02x %02x %02x %02x | %02x %02x %02x %02x %02x %02x %02x %02x",
    base, m[16 * i], m[16 * i + 1], m[16 * i + 2], m[16 * i + 3], m[16 * i + 4], m[16 * i + 5], m[16 * i + 6],
    m[16 * i + 7], m[16 * i + 8], m[16 * i + 9],m[16 * i + 10], m[16 * i + 11], m[16 * i + 12], m[16 * i + 13],
    m[16 * i + 14], m[16 * i + 15]);
    Serial.println(line);
  }
  int base = startAddress + 16 * fulllines;
  sprintf(line, "%04x:", base);
  char temp[16];
  for (int j = 0; j < left; j++) {
    if (j == 8) {
      sprintf(temp, " | %02x", m[16 * fulllines + j]);
    } else {
      sprintf(temp, " %02x", m[16 * fulllines + j]);
    }
    strcat(line, temp);
  }
  if (left) {
    Serial.println(line);
  }
  Serial.println("-------------------------------------------------------");
  return;
}

//reads sequential bytes, storing result in an array
void memByteStreamRead(int na, int startAddress, byte* m, int numBytes) {
  //wish to read from memory
  digitalWrite(RW, HIGH);
  for (int i = 0; i < numBytes; i++) {
    m[i] = memByteRead(na, startAddress + i);
  }
  return;
}

//reads a byte of data from a specified na-bit address
byte memByteRead(int na, int add) {
  //pull latch high
  digitalWrite(SLA, HIGH);
  int pow2 = 1 << na;
  int modded = add % pow2;
  int sets8 = na / 8 + 1;
  int offset = 8 - na % 8;
  modded = modded << offset;
  byte m[sets8];
  for (int i = 0; i < sets8; i++) {
    m[i] = (modded >> 8 * (sets8 - i - 1)) % 256;
  }
  sendByteStream(m);
  //pull latch low
  digitalWrite(SLA, LOW);
  byte rd = readByte();
  digitalWrite(SLA, HIGH);
  return rd;
}

//makes for a cool light show!
void binaryCtr(int d) {
  byte m[] = {0};
  while (true) {
    sendByteStream(m);
    delay(d);
    m[0] += 1;
  }
  return;
}

//sends a byte stream of data via shift regs
//least significant byte first
void sendByteStream(byte* b) {
  int n = sizeof(b) / sizeof(b[0]);
  for (int i = 1; i <= n ; i++) {
    sendByte(b[n - i]);
  }
  return;
}

//sends a byte of data via shift regs
//least significant bit first
void sendByte(byte b) {
  for (int i = 0; i < 8; i++) {
    int j = b & 1;
    if (j) {
      digitalWrite(SDA, HIGH);
    } else {
      digitalWrite(SDA, LOW);
    }
    pulseClock(0);
    b = b >> 1;
  }
  return;
}

//reads a byte in parallel
byte readByte() {
  int ports[] = {D7, D6, D5, D4, D3, D2, D1, D0};
  byte r = 0;
  for (int i = 0; i < 8; i++) {
    r <<= 1;
    r |= digitalRead(ports[i]);
  }
  return r;
}

//set data first, then pulse to shift
void pulseClock(int d) {
  digitalWrite(SCL, HIGH);
  delay(d);
  digitalWrite(SCL, LOW);
  return;
}
