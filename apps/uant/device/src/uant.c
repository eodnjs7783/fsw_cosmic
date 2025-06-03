#include <uant/uant.h>
#include <HYVRID/hyvrid.h>

#include <string.h>
#include <stdint.h>


int ISIS_UANT_SendCmd(uint8_t cc) {
	return isis_uant_i2c_write(cc, 0, true);
}

int ISIS_UANT_SendCmdWithResponse(uint8_t cc,
								  uint8_t respLen,
								  void* resp) {
	return isis_uant_i2c_read(cc, resp, respLen);
}

int ISIS_UANT_SendCmdWithParam(uint8_t cc,
							   uint8_t param) {
	return isis_uant_i2c_write(cc, param, false);
}

int ISIS_UANT_MeasureAntennaSystemTemperature(uint16_t *rawVoltage) {
	if (!rawVoltage) {
		return DEVICE_ERR_NULL;
	}
	return ISIS_UANT_SendCmdWithResponse(UANT_CMD_MEASURE_SYSTEM_TEMPERATURE,
										 sizeof(*rawVoltage),
										 rawVoltage);
}

int ISIS_UANT_ReportDeploymentStatus(uint16_t *DeploymentStatus) {
	if (!DeploymentStatus) {
		return DEVICE_ERR_NULL;
	}
	return ISIS_UANT_SendCmdWithResponse(UANT_CMD_REPORT_DEPLOYMENT_STATUS,
										 sizeof(*DeploymentStatus),
										 DeploymentStatus);
}

int ISIS_UANT_ReportAntennaActivationCount(uint8_t AntennaNumber,
										   uint8_t* ActivationCount) {
	uint8_t reg;

	if (!ActivationCount) {
		return DEVICE_ERR_NULL;
	}

	switch (AntennaNumber) {
		case 1: reg = UANT_CMD_REPORT_ANT1_ACTIVATION_CNT;
			break;
		case 2: reg = UANT_CMD_REPORT_ANT2_ACTIVATION_CNT;
			break;
		case 3: reg = UANT_CMD_REPORT_ANT3_ACTIVATION_CNT;
			break;
		case 4: reg = UANT_CMD_REPORT_ANT4_ACTIVATION_CNT;
			break;
		break;
			return DEVICE_ERR_RANGE;
	}

	return ISIS_UANT_SendCmdWithResponse(reg,
										 sizeof(*ActivationCount),
										 ActivationCount);
}

int ISIS_UANT_ReportAntennaActivationTime(uint8_t AntennaNumber,
										  uint16_t* ActivationTime) {
	uint8_t reg;

	if (!ActivationTime) {
		return DEVICE_ERR_NULL;
	}

	switch (AntennaNumber) {
		case 1: reg = UANT_CMD_REPORT_ANT1_ACTIVATION_TIME;
			break;
		case 2: reg = UANT_CMD_REPORT_ANT2_ACTIVATION_TIME;
			break;
		case 3: reg = UANT_CMD_REPORT_ANT3_ACTIVATION_TIME;
			break;
		case 4: reg = UANT_CMD_REPORT_ANT4_ACTIVATION_TIME;
			break;
		break;
			return DEVICE_ERR_RANGE;
	}

	/**
	 * The user manual seems to claim that the LSB comes first (LE).
	 * If not we need a ntohs() here.
	 */
	return ISIS_UANT_SendCmdWithResponse(reg,
									 	 sizeof(*ActivationTime),
									 	 ActivationTime);
}
