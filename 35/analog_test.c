// ── TCRT5000 感測器 PIN 定義 ─────────────────────────────────────
#define IR_LEFT   34
#define IR_MID    35
#define IR_RIGHT  32

void setup() {
  // 初始化序列埠，建議使用 115200 速率
  Serial.begin(115200);
  
  pinMode(IR_LEFT, INPUT);
  pinMode(IR_MID, INPUT);
  pinMode(IR_RIGHT, INPUT);

  Serial.println("========================================");
  Serial.println("TCRT5000 類比數值測試開始...");
  Serial.println("請將感測器放在黑線與白地上觀察數值變化");
  Serial.println("========================================");
  delay(1000);
}

void loop() {
  // 讀取類比值 (ESP32 預設範圍為 0 ~ 4095)
  int valL = analogRead(IR_LEFT);
  int valM = analogRead(IR_MID);
  int valR = analogRead(IR_RIGHT);

  // 計算總和 (用於判斷是否脫線)
  int sum = valL + valM + valR;

  // 格式化輸出到 Serial Monitor
  Serial.print("L: "); Serial.print(valL);
  Serial.print("\tM: "); Serial.print(valM);
  Serial.print("\tR: "); Serial.print(valR);
  Serial.print("\t| SUM: "); Serial.println(sum);

  // 如果你想在「序列繪圖器 (Serial Plotter)」看波形，可以取消下面這行的註解：
  // Serial.printf("%d,%d,%d\n", valL, valM, valR);

  delay(100); // 每 0.1 秒更新一次
}
