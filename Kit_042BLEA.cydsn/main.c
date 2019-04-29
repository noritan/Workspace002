/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
#include <project.h>

// Major/Minor フィールドの位置
#define     MAJOR_OFFSET    (26u)
#define     MINOR_OFFSET    (28u)

// Minor フィールドの最小・最大値および変化ステップ
#define     MINOR_MIN       (69u)   //  0℃相当の値
#define     MINOR_MAX       (126u)  // 40℃相当の値
#define     MINOR_STEP      (1u)    // Minor値の変化量

// Major/Minor フィールドの値
uint8   f_minor = MINOR_MIN;        // Minorフィールドの値
uint8   f_major = 0;                // Majorフィールドの値

// BLE の初期化終了フラグ
uint8   initialized = 0;

// ソフトウェアタイマのカウンタ
uint8   tick = 0;

// 500msごとに Advertisement パケットを開始・停止する
void Wdt_Callback(void) {
    if (initialized) {
        // 初期化がされた
        if (tick == 0) {
            // 最初の500msで Advertisement パケットを送信する
            
            // Major フィールドの設定
            cyBle_discoveryData.advData[MAJOR_OFFSET] = f_major;
            
            // Minor フィールドの設定
            if ((f_minor < MINOR_MIN) || (f_minor > MINOR_MAX)) {
                f_minor = MINOR_MIN;
            }
            cyBle_discoveryData.advData[MINOR_OFFSET] = f_minor;
            f_minor += MINOR_STEP;

            // Advertisement パケットの送信を開始する
            CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
            tick = 1;
        } else {
            // 次の500msで Advertisement ポケットの送信を止める
            CyBle_GappStopAdvertisement();
            tick = 0;
        }
    }
}

// BLE スタックの処理ハンドラ
void StackEventHandler(uint32 event, void *eventParam) {
    switch (event) {
        // スタックが立ちあがったまたはデバイスが切断されたら初期化終了とみなす
        case CYBLE_EVT_STACK_ON:
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            initialized = 1;
            break;
        // それ以外のイベントは無視する
        default:
            break;
    }
}

// メインループ
int main() {
    CYBLE_API_RESULT_T apiResult;       // エラーコード

    // 割り込みを許可する
    CyGlobalIntEnable;
    
    // 低速タイマ Timer0 のコールバック関数を設定する
    CySysWdtSetInterruptCallback(CY_SYS_WDT_COUNTER0, Wdt_Callback);

    // BLE デバイスの初期化を行う
    apiResult = CyBle_Start(StackEventHandler);
    // 初期化が正常に終わったかを確認する
    CYASSERT(apiResult == CYBLE_ERROR_OK);

    for(;;){
        // BLE スタックへのイベントを処理する
        CyBle_ProcessEvents();
    }
}

/* [] END OF FILE */
