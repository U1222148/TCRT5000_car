// ── 前輪馬達 PIN（維持手動測試正常的腳位） ─────────────────────
#define forward_ENABLE_A  15  
#define forward_ENABLE_B  5   
#define forward_IN1       4   
#define forward_IN2       2   
#define forward_IN3       17  
#define forward_IN4       16  

// ── 後輪馬達 PIN ────────────────────────────────────────────
#define back_ENABLE_A     25  
#define back_ENABLE_B     13  
#define back_IN1          27  
#define back_IN2          26  
#define back_IN3          12  
#define back_IN4          14  

// ── TCRT5000 感測器 PIN ─────────────────────────────────────
#define IR_LEFT   34
#define IR_MID    35
#define IR_RIGHT  32

// ── 參數設定（大馬力落地極限優化） ───────────────────────────
const int THRESHOLD = 2200;  // 稍微調高門檻值，避免環境雜訊誤判
const int BASE_SPEED = 80;   // 落地基礎速度（剛好能動最安全）
const int TURN_SPEED = 105;   // 外側輪修正速度（加大推力）
const int BRAKE_SPEED = 75;  // 內側輪「反轉主動煞車」的力量！

void setup() {
  // 徹底關閉 Serial 功能，不佔用任何 CPU 執行時間
  
  pinMode(forward_IN1, OUTPUT); pinMode(forward_IN2, OUTPUT);
  pinMode(forward_IN3, OUTPUT); pinMode(forward_IN4, OUTPUT);
  pinMode(forward_ENABLE_A, OUTPUT); pinMode(forward_ENABLE_B, OUTPUT);
  
  pinMode(back_IN1, OUTPUT);    pinMode(back_IN2, OUTPUT);
  pinMode(back_IN3, OUTPUT);    pinMode(back_IN4, OUTPUT);
  pinMode(back_ENABLE_A, OUTPUT); pinMode(back_ENABLE_B, OUTPUT);

  pinMode(IR_LEFT, INPUT);
  pinMode(IR_MID, INPUT);
  pinMode(IR_RIGHT, INPUT);

  // 落地準備緩衝
  delay(3000); 
}

void loop() {
  // 零延遲極速讀取
  int valL = analogRead(IR_LEFT);
  int valM = analogRead(IR_MID);
  int valR = analogRead(IR_RIGHT);

  // ── 核心邏輯：加入反轉主動煞車 ──
  
  if (valM > THRESHOLD && valL <= THRESHOLD && valR <= THRESHOLD) {
    // 中間踩線 -> 直行
    // 左側前進
    digitalWrite(forward_IN3, HIGH); digitalWrite(forward_IN4, LOW);
    digitalWrite(back_IN1,    HIGH); digitalWrite(back_IN2,    LOW);
    // 右側前進
    digitalWrite(forward_IN1, HIGH); digitalWrite(forward_IN2, LOW);
    digitalWrite(back_IN3,    HIGH); digitalWrite(back_IN4, LOW);

    analogWrite(forward_ENABLE_B, BASE_SPEED); 
    analogWrite(back_ENABLE_A,    BASE_SPEED); 
    analogWrite(forward_ENABLE_A, BASE_SPEED); 
    analogWrite(back_ENABLE_B,    BASE_SPEED); 
  } 
  else if (valL > THRESHOLD) {
    // 左邊踩線 -> 車子偏右 -> 右輪全力前進，左輪「主動反轉」強制拉回！
    
    // 左側反轉 (HIGH/LOW 對調)
    digitalWrite(forward_IN3, LOW);  digitalWrite(forward_IN4, HIGH);
    digitalWrite(back_IN1,    LOW);  digitalWrite(back_IN2,    HIGH);
    // 右側前進
    digitalWrite(forward_IN1, HIGH); digitalWrite(forward_IN2, LOW);
    digitalWrite(back_IN3,    HIGH); digitalWrite(back_IN4, LOW);

    analogWrite(forward_ENABLE_B, BRAKE_SPEED); // 左側給予反轉力道
    analogWrite(back_ENABLE_A,    BRAKE_SPEED); 
    analogWrite(forward_ENABLE_A, TURN_SPEED);  // 右側全力推進
    analogWrite(back_ENABLE_B,    TURN_SPEED); 
  } 
  else if (valR > THRESHOLD) {
    // 右邊踩線 -> 車子偏左 -> 左輪全力前進，右輪「主動反轉」強制拉回！
    
    // 左側前進
    digitalWrite(forward_IN3, HIGH); digitalWrite(forward_IN4, LOW);
    digitalWrite(back_IN1,    HIGH); digitalWrite(back_IN2,    LOW);
    // 右側反轉 (HIGH/LOW 對調)
    digitalWrite(forward_IN1, LOW);  digitalWrite(forward_IN2, HIGH);
    digitalWrite(back_IN3,    LOW);  digitalWrite(back_IN4, HIGH);

    analogWrite(forward_ENABLE_B, TURN_SPEED);  // 左側全力推進
    analogWrite(back_ENABLE_A,    TURN_SPEED); 
    analogWrite(forward_ENABLE_A, BRAKE_SPEED); // 右側給予反轉力道
    analogWrite(back_ENABLE_B,    BRAKE_SPEED); 
  } 
  else if (valL <= THRESHOLD && valM <= THRESHOLD && valR <= THRESHOLD) {
    // 全脫線 -> 瞬間鎖死
    digitalWrite(forward_IN1, LOW); digitalWrite(forward_IN2, LOW);
    digitalWrite(forward_IN3, LOW); digitalWrite(forward_IN4, LOW);
    digitalWrite(back_IN1,    LOW); digitalWrite(back_IN2,    LOW);
    digitalWrite(back_IN3,    LOW); digitalWrite(back_IN4, LOW);
    
    analogWrite(forward_ENABLE_A, 0); analogWrite(forward_ENABLE_B, 0);
    analogWrite(back_ENABLE_A,    0); analogWrite(back_ENABLE_B,    0);
  }
}
