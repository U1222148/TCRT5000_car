// ── 前輪馬達 PIN ────────────────────────────────────────────
#define forward_ENABLE_A  5
#define forward_ENABLE_B  15
#define forward_IN1       17
#define forward_IN2       16
#define forward_IN3       4
#define forward_IN4       2

// ── 後輪馬達 PIN ────────────────────────────────────────────
#define back_ENABLE_B     25
#define back_ENABLE_A     13
#define back_IN1          12
#define back_IN2          14
#define back_IN3          27
#define back_IN4          26

void setup() {
  Serial.begin(115200);
  
  // 設定所有馬達引腳為輸出
  pinMode(forward_IN1, OUTPUT); pinMode(forward_IN2, OUTPUT);
  pinMode(forward_IN3, OUTPUT); pinMode(forward_IN4, OUTPUT);
  pinMode(forward_ENABLE_A, OUTPUT); pinMode(forward_ENABLE_B, OUTPUT);
  
  pinMode(back_IN1, OUTPUT);    pinMode(back_IN2, OUTPUT);
  pinMode(back_IN3, OUTPUT);    pinMode(back_IN4, OUTPUT);
  pinMode(back_ENABLE_A, OUTPUT); pinMode(back_ENABLE_B, OUTPUT);

  Serial.println("警告：馬達即將全速運轉！請架高車身避免噴飛。");
  delay(3000); // 緩衝 3 秒，讓你有時間反應
}

void loop() {
  Serial.println("馬達全速向前運行中...");

  // --- 前左輪 (forward_ENABLE_B) ---
  digitalWrite(forward_IN3, HIGH); 
  digitalWrite(forward_IN4, LOW); 
  analogWrite(forward_ENABLE_A, 150);

  // --- 前右輪 (forward_ENABLE_A) ---
  digitalWrite(forward_IN1, HIGH); 
  digitalWrite(forward_IN2, LOW); 
  analogWrite(forward_ENABLE_B, 150);

  // --- 後左輪 (back_ENABLE_A) ---
  digitalWrite(back_IN1, HIGH); 
  digitalWrite(back_IN2, LOW); 
  analogWrite(back_ENABLE_B, 150);

  // --- 後右輪 (back_ENABLE_B) ---
  digitalWrite(back_IN3, HIGH); 
  digitalWrite(back_IN4, LOW); 
  analogWrite(back_ENABLE_A, 150);

  delay(1000); 
}
