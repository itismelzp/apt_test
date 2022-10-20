package com.ccand99.apt_compiler

import javax.lang.model.element.Element

/**
 * Created by lizhiping on 2022/10/14.
 * <p>
 * description
 */
data class AutelConvertInfo(
    var element: Element? = null,
    val keyName: String,
    var canSet: Boolean = false,
    var canGet: Boolean = false,
    var canAction: Boolean = false,
    var canListen: Boolean = false,
    var paramConverter: String? = "",
    var resultConverter: String? = "",
    var paramBean: String = "",
    var paramMsg: String = "",
    var resultBean: String = "",
    var resultMsg: String = ""
)
