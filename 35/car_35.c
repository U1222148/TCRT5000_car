
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

// ── TCRT5000 感測器 PIN ─────────────────────────────────────
#define IR_LEFT   34
#define IR_MID    35
#define IR_RIGHT  32

// ── 巡跡與安全參數設定 ────────────────────────────────────────────
const int THRESHOLD = 1500;   // 閥值

const int CRUISE_SPEED = 150; // 直走速度
const int TURN_SPEED = 180;   // 轉彎修正速度

// 【脫線安全設定】
// 容許脫線的最大時間（單位：毫秒）。在這個時間內車子會嘗試扭回軌道，超過就自動停車。
const unsigned long MAX_LOST_TIME = 150; 

int last_direction = 0;       // 紀錄上一次的偏向 (1=偏左, 2=偏右, 0=正中)
bool is_lost = false;         // 紀錄目前是否處於脫線狀態
unsigned long lost_start_time = 0; // 開始脫線的時間戳記

void setup() {
  // 馬達引腳初始化
  pinMode(forward_IN1, OUTPUT); pinMode(forward_IN2, OUTPUT);
  pinMode(forward_IN3, OUTPUT); pinMode(forward_IN4, OUTPUT);
  pinMode(forward_ENABLE_A, OUTPUT); pinMode(forward_ENABLE_B, OUTPUT);
  
  pinMode(back_IN1, OUTPUT);    pinMode(back_IN2, OUTPUT);
  pinMode(back_IN3, OUTPUT);    pinMode(back_IN4, OUTPUT);
  pinMode(back_ENABLE_A, OUTPUT); pinMode(back_ENABLE_B, OUTPUT);

  // 感測器引腳初始化
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_MID, INPUT);
  pinMode(IR_RIGHT, INPUT);

  delay(3000); // 開機緩衝
}

void loop() {
  // 1. 高速讀取感測器
  bool L = (analogRead(IR_LEFT) > THRESHOLD);
  bool M = (analogRead(IR_MID) > THRESHOLD);
  bool R = (analogRead(IR_RIGHT) > THRESHOLD);

  // 2. 巡跡核心控制邏輯
  if (M && !L && !R) {
    // [0, 1, 0] 在正軌上
    moveForward(CRUISE_SPEED);
    last_direction = 0;
    is_lost = false; // 重設脫線狀態
  } 
  else if (L && !R) {
    // [1, 1, 0] 或 [1, 0, 0] 左邊踩線
    turnRight_Spin(TURN_SPEED); 
    last_direction = 1;
    is_lost = false; // 重設脫線狀態
  } 
  else if (R && !L) {
    // [0, 1, 1] 或 [0, 0, 1] 右邊踩線
    turnLeft_Spin(TURN_SPEED);
    last_direction = 2;
    is_lost = false; // 重設脫線狀態
  } 
  else if (L && M && R) {
    // [1, 1, 1] 全黑十字路口
    moveForward(CRUISE_SPEED);
    is_lost = false; // 重設脫線狀態
  }
  else if (!L && !M && !R) {
    // 🚨 [0, 0, 0] 全白！觸發脫線判定
    
    if (!is_lost) {
      // 剛踏入全白的第一瞬間，啟動計時器
      is_lost = true;
      lost_start_time = millis(); // 紀錄當前的時間（毫秒）
    }

    // 檢查脫線時間是否超過上限
    if (millis() - lost_start_time > MAX_LOST_TIME) {
      // 超過時間了，判定徹底離開軌道 -> 安全停車！
      motorStop();
    } else {
      // 在容許時間內，嘗試用最後的記憶原地旋轉扭回來
      if (last_direction == 1) {
        turnRight_Spin(TURN_SPEED);
      } else if (last_direction == 2) {
        turnLeft_Spin(TURN_SPEED);
      } else {
        motorStop(); // 如果本來就沒有方向記憶，直接停
      }
    }
  }
}

// ── 馬達驅動子程式（原地旋轉 Spin 版） ──────────────────────────

void moveForward(int speed) {
  digitalWrite(forward_IN3, LOW);  digitalWrite(forward_IN4, HIGH);  analogWrite(forward_ENABLE_B, speed);
  digitalWrite(forward_IN1, LOW);  digitalWrite(forward_IN2, HIGH);  analogWrite(forward_ENABLE_A, speed);
  digitalWrite(back_IN1, HIGH);    digitalWrite(back_IN2, LOW);   analogWrite(back_ENABLE_A, speed);
  digitalWrite(back_IN3, HIGH);    digitalWrite(back_IN4, LOW);   analogWrite(back_ENABLE_B, speed);
}

void turnLeft_Spin(int speed) {
  digitalWrite(forward_IN3, HIGH); digitalWrite(forward_IN4, LOW);  analogWrite(forward_ENABLE_B, speed);
  digitalWrite(back_IN1, LOW);     digitalWrite(back_IN2, HIGH); analogWrite(back_ENABLE_A, speed);
  digitalWrite(forward_IN1, LOW);  digitalWrite(forward_IN2, HIGH);  analogWrite(forward_ENABLE_A, speed);
  digitalWrite(back_IN3, HIGH);    digitalWrite(back_IN4, LOW);   analogWrite(back_ENABLE_B, speed);
}

void turnRight_Spin(int speed) {
  digitalWrite(forward_IN3, LOW);  digitalWrite(forward_IN4, HIGH);  analogWrite(forward_ENABLE_B, speed);
  digitalWrite(back_IN1, HIGH);    digitalWrite(back_IN2, LOW);   analogWrite(back_ENABLE_A, speed);
  digitalWrite(forward_IN1, HIGH); digitalWrite(forward_IN2, LOW);  analogWrite(forward_ENABLE_A, speed);
  digitalWrite(back_IN3, LOW);     digitalWrite(back_IN4, HIGH); analogWrite(back_ENABLE_B, speed);
}

void motorStop() {
  analogWrite(forward_ENABLE_A, 0);
  analogWrite(forward_ENABLE_B, 0);
  analogWrite(back_ENABLE_A, 0);
  analogWrite(back_ENABLE_B, 0);
}
