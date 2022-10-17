package com.ccand99.apt_annotations

import kotlin.reflect.KClass

/**
 * Created by lizhiping on 2022/10/1.
 * <p>
 * description
 */

@Target(AnnotationTarget.FIELD)
@Retention(AnnotationRetention.BINARY)
annotation class AutelConverter(
    val keyName: String,
    val canSet: Boolean = false,
    val canGet: Boolean = false,
    val canAction: Boolean = false,
    val canListen: Boolean = false,
    val paramConverter: KClass<*>,
    val paramBean: String = "",
    val paramMsg: String = "",
    val resultConverter: String = "",
    val resultBea: String = "",
    val resultMsg: String = ""
)
