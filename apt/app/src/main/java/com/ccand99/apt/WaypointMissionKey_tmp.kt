//package com.ccand99.apt
//
//import com.autel.drone.sdk.pbprotocol.interaction.constants.MessageTypeConstant
//import com.autel.drone.sdk.vmodelx.manager.keyvalue.value.waypoint.bean.MissionWaypointBreakRspBean
//import com.autel.drone.sdk.vmodelx.manager.keyvalue.value.waypoint.bean.MissionWaypointGUIDBean
//import com.autel.drone.sdk.vmodelx.manager.keyvalue.value.waypoint.bean.MissionWaypointStatusReportNtfyBean
//import com.autel.drone.sdk.vmodelx.manager.keyvalue.converter.AutelEmptyConvert
//import com.autel.drone.sdk.vmodelx.manager.keyvalue.value.waypoint.converter.WaypointGUIDConverter
//import com.autel.drone.sdk.vmodelx.manager.keyvalue.value.waypoint.converter.WaypointBreakRspConverter
//import com.autel.drone.sdk.vmodelx.manager.keyvalue.value.waypoint.converter.WaypointStatusReportNtfyConverter
//import com.autel.drone.sdk.vmodelx.manager.keyvalue.key.AutelActionKeyInfo
//import com.autel.drone.sdk.vmodelx.manager.keyvalue.key.AutelKeyInfo
//import com.autel.drone.sdk.vmodelx.manager.keyvalue.key.ComponentType
//
//
///**
// * Created by lizhiping on 2022/9/23.
// * <p>
// * 航点任务包装类
// */
//object WaypointMissionKey {
//
//    val component = ComponentType.MISSION
//    val KeyEnter: AutelActionKeyInfo<Void, Void> = AutelActionKeyInfo(
//        component.value,
//        MessageTypeConstant.MISSION_WAYPOINT_ENTER_MSG,
//        AutelEmptyConvert(),
//        AutelEmptyConvert()
//    ).canPerformAction(true)
//
//    val KeyStop: AutelActionKeyInfo<Void, Void> = AutelActionKeyInfo(
//        component.value,
//        MessageTypeConstant.MISSION_WAYPOINT_STOP_MSG,
//        AutelEmptyConvert(),
//        AutelEmptyConvert()
//    ).canPerformAction(true)
//
//
//    val KeyStatusReportNtfy: AutelKeyInfo<MissionWaypointStatusReportNtfyBean> = AutelKeyInfo(
//        component.value,
//        MessageTypeConstant.MISSION_WAYPOINT_STATUS_REPORT_NTFY,
//        WaypointStatusReportNtfyConverter()
//    ).canListen(true)
//}