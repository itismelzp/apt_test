package com.ccand99.apt

import com.ccand99.apt_annotations.AutelConverter

/**
 * @date 2022/9/23.
 * @author maowei
 * @description TODO
 */
object MessageTypeConstant {

    const val GET = "get"
    const val SET = "set"

    // 航点任务
    @AutelConverter(
        keyName = "MessageTypeConstant.MISSION_WAYPOINT_ENTER_MSG",
        canGet = true,
        canSet = true,
        paramConverter = "AutelEmptyConvert",
        resultBean = "MissionWaypointStatusReportNtfyBean",
        resultConverter = "AutelEmptyConvert"
    )
    const val MISSION_WAYPOINT_ENTER_MSG                  = "enterMission"     //进入航点任务功能，对应消息结构：无

    @AutelConverter(
        keyName = "MessageTypeConstant.MISSION_WAYPOINT_EXIT_MSG",
        canGet = true,
        canAction = true,
        paramConverter = "AutelEmptyConvert",
        resultBean = "MissionWaypointStatusReportNtfyBean",
        resultConverter = "AutelEmptyConvert"
    )
    const val MISSION_WAYPOINT_EXIT_MSG                   = "exitMission"     //退出航点任务，对应消息结构：无
//    @AutelConverter(canSet = true, canGet= true, canAction = true, canListen = true, paramConverter = "ParamConverter", resultConverter = "ResultConverter", paramBean = clazz, paramMsg = clazz, resultBean = clazz, resultMsg = clazz)
    const val MISSION_WAYPOINT_START_MSG                  = "startMission"     //开始航点任务，对应消息结构：MissionWaypointGUID
    const val MISSION_WAYPOINT_PAUSE_MSG                  = "pauseMission"     //暂停航点任务，对应消息结构：无
    const val MISSION_WAYPOINT_CONTINUE_MSG               = "resumeMission"     //继续航点任务，对应消息结构：MissionWaypointGUID
    const val MISSION_WAYPOINT_STOP_MSG                   = "stopMission"     //停止航点任务，对应消息结构：无
    const val MISSION_WAYPOINT_BREAK_REQUEST_MSG          = "breakRequest"     //请求断点信息，对应消息结构：MissionWaypointBreakRsp,MissionWaypointGUID
    const val MISSION_WAYPOINT_STATUS_REPORT_NTFY         = "statusReport"

}