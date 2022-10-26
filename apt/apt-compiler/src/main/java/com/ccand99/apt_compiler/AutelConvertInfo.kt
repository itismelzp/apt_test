package com.ccand99.apt_compiler

import javax.lang.model.element.Element
import kotlin.reflect.KClass

/**
 * Created by lizhiping on 2022/10/14.
 * <p>
 * description
 */
data class AutelConvertInfo(
    var element: Element,
    val keyName: String,
    var canSet: Boolean = false,
    var canGet: Boolean = false,
    var canAction: Boolean = false,
    var canListen: Boolean = false,
    var paramConverter: String? = "",
    var resultConverter: String? = "",
    var paramBean: KClass<out Any>? = null,
    var paramMsg: String = "",
    var resultBean: KClass<out Any>? = null,
    var resultMsg: String = ""
)
