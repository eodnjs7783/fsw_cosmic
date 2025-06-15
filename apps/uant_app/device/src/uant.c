#include <stdint.h>
#include <stdbool.h>
#include <cfe_srl.h>
#include <uant/uant.h>


// I2C 통신을 위한 핸들 인덱서 정의
#ifndef UANT_I2C_HANDLE_INDEXER
#define UANT_I2C_HANDLE_INDEXER CFE_SRL_I2C0_HANDLE_INDEXER
#endif

// 전역 I2C 핸들 포인터 (초기값 NULL)
static CFE_SRL_IO_Handle_t *I2C_Handle = NULL;

/**
 * @brief I2C 핸들을 반환하거나 초기화
 * @return 초기화된 I2C 핸들 포인터
 * @details 최초 호출 시 CFE_SRL_ApiGetHandle로 핸들을 가져오고,
 *          이후에는 캐싱된 핸들을 반환하여 중복 초기화를 방지합니다.
 */
static inline CFE_SRL_IO_Handle_t* Get_I2C_Handle(void) {
    if (I2C_Handle == NULL) {
        I2C_Handle = CFE_SRL_ApiGetHandle(UANT_I2C_HANDLE_INDEXER);
    }
    return I2C_Handle;
}

/**
 * @brief 커맨드만 전송 (레지스터 주소만)
 * @param cc    전송할 커맨드 코드 (레지스터)
 * @return DEVICE_SUCCESS 성공, DEVICE_ERROR 실패
 */
int ISIS_UANT_SendCmd(uint8_t cc) {
    CFE_SRL_IO_Param_t params = {0};
    uint8_t reg = cc;  // 전송할 레지스터 주소

    params.TxData  = &reg;    // Tx 버퍼: 레지스터
    params.TxSize  = 1;       // 버퍼 크기: 1바이트
    params.Timeout = 10;      // 타임아웃: 10ms

    // I2C 쓰기 수행
    int32 status = CFE_SRL_ApiWrite(Get_I2C_Handle(), &params);
    return (status == CFE_SRL_OK ? DEVICE_SUCCESS : DEVICE_ERROR);
}

/**
 * @brief 커맨드 + 파라미터 전송
 * @param cc     전송할 커맨드 코드
 * @param param  커맨드에 붙일 파라미터 바이트
 * @return DEVICE_SUCCESS 성공, DEVICE_ERROR 실패
 */
int ISIS_UANT_SendCmdWithParam(uint8_t cc, uint8_t param) {
    CFE_SRL_IO_Param_t params = {0};
    uint8_t buf[2] = { cc, param };  // 레지스터 + 파라미터

    params.TxData  = buf;         // Tx 버퍼 설정
    params.TxSize  = sizeof(buf); // 버퍼 크기: 2바이트
    params.Timeout = 10;          // 타임아웃: 10ms

    // I2C 쓰기 수행
    int32 status = CFE_SRL_ApiWrite(Get_I2C_Handle(), &params);
    return (status == CFE_SRL_OK ? DEVICE_SUCCESS : DEVICE_ERROR);
}

/**
 * @brief 커맨드 전송 후 응답 읽기
 * @param cc       전송할 커맨드 코드
 * @param respLen  읽어올 응답 바이트 수
 * @param resp     읽은 데이터를 저장할 버퍼 포인터
 * @return DEVICE_SUCCESS 성공, DEVICE_ERR_READ 읽기 실패
 */
int ISIS_UANT_SendCmdWithResponse(uint8_t cc, uint8_t respLen, void* resp) {
    CFE_SRL_IO_Param_t params = {0};
    uint8_t reg = cc;  // 전송할 레지스터 주소

    params.TxData  = &reg;        // Tx: 레지스터 전송
    params.TxSize  = 1;           // 크기: 1바이트
    params.RxData  = (uint8_t*)resp;  // Rx: 응답 버퍼
    params.RxSize  = respLen;        // 읽을 바이트 수
    params.Timeout = 100;            // 타임아웃: 100ms

    // I2C 읽기 수행
    int32 status = CFE_SRL_ApiRead(Get_I2C_Handle(), &params);
    return (status == CFE_SRL_OK ? DEVICE_SUCCESS : DEVICE_ERR_READ);
}

/**
 * @brief 안테나 시스템 온도 측정
 * @param rawVoltage  측정된 전압(raw) 저장 포인터
 * @return DEVICE_SUCCESS 성공, DEVICE_ERR_NULL NULL 포인터, DEVICE_ERR_READ 읽기 실패
 */
int ISIS_UANT_MeasureAntennaSystemTemperature(uint16_t *rawVoltage) {
    if (!rawVoltage) {
        return DEVICE_ERR_NULL;  // NULL 포인터 검증
    }
    // 응답 버퍼 크기: uint16_t (2바이트)
    return ISIS_UANT_SendCmdWithResponse(
        UANT_CMD_MEASURE_SYSTEM_TEMPERATURE,
        sizeof(*rawVoltage),
        rawVoltage
    );
}

/**
 * @brief 전개 상태 리포트
 * @param DeploymentStatus  상태 값을 저장할 포인터
 * @return DEVICE_SUCCESS 성공, DEVICE_ERR_NULL NULL 포인터, DEVICE_ERR_READ 읽기 실패
 */
int ISIS_UANT_ReportDeploymentStatus(uint16_t *DeploymentStatus) {
    if (!DeploymentStatus) {
        return DEVICE_ERR_NULL;  // NULL 포인터 검증
    }
    return ISIS_UANT_SendCmdWithResponse(
        UANT_CMD_REPORT_DEPLOYMENT_STATUS,
        sizeof(*DeploymentStatus),
        DeploymentStatus
    );
}

/**
 * @brief 안테나별 활성화 횟수 리포트
 * @param AntennaNumber    안테나 번호 (1~4)
 * @param ActivationCount  활성화 횟수를 저장할 포인터
 * @return DEVICE_SUCCESS 성공, DEVICE_ERR_NULL NULL 포인터,
 *         DEVICE_ERR_RANGE 범위 벗어남, DEVICE_ERR_READ 읽기 실패
 */
int ISIS_UANT_ReportAntennaActivationCount(uint8_t AntennaNumber,
                                           uint8_t* ActivationCount) {
    if (!ActivationCount) {
        return DEVICE_ERR_NULL;  // NULL 포인터 검증
    }
    uint8_t reg;
    // 안테나 번호에 따른 레지스터 매핑
    switch (AntennaNumber) {
        case 1: reg = UANT_CMD_REPORT_ANT1_ACTIVATION_CNT; break;
        case 2: reg = UANT_CMD_REPORT_ANT2_ACTIVATION_CNT; break;
        case 3: reg = UANT_CMD_REPORT_ANT3_ACTIVATION_CNT; break;
        case 4: reg = UANT_CMD_REPORT_ANT4_ACTIVATION_CNT; break;
        default:
            return DEVICE_ERR_RANGE;  // 범위 오류
    }
    return ISIS_UANT_SendCmdWithResponse(
        reg,
        sizeof(*ActivationCount),
        ActivationCount
    );
}

/**
 * @brief 안테나별 활성화 시간 리포트
 * @param AntennaNumber     안테나 번호 (1~4)
 * @param ActivationTime    활성화 시간을 저장할 포인터 (uint16_t)
 * @return DEVICE_SUCCESS 성공, DEVICE_ERR_NULL NULL 포인터,
 *         DEVICE_ERR_RANGE 범위 벗어남, DEVICE_ERR_READ 읽기 실패
 */
int ISIS_UANT_ReportAntennaActivationTime(uint8_t AntennaNumber,
                                          uint16_t* ActivationTime) {
    if (!ActivationTime) {
        return DEVICE_ERR_NULL;  // NULL 포인터 검증
    }
    uint8_t reg;
    // 안테나 번호에 따른 레지스터 매핑
    switch (AntennaNumber) {
        case 1: reg = UANT_CMD_REPORT_ANT1_ACTIVATION_TIME; break;
        case 2: reg = UANT_CMD_REPORT_ANT2_ACTIVATION_TIME; break;
        case 3: reg = UANT_CMD_REPORT_ANT3_ACTIVATION_TIME; break;
        case 4: reg = UANT_CMD_REPORT_ANT4_ACTIVATION_TIME; break;
        default:
            return DEVICE_ERR_RANGE;  // 범위 오류
    }
    // LSB 우선 전송(리틀 엔디언) 가정. 필요 시 ntohs() 적용
    return ISIS_UANT_SendCmdWithResponse(
        reg,
        sizeof(*ActivationTime),
        ActivationTime
    );
}
