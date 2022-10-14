package com.ccand99.apt_annotations

/**
 * Created by lizhiping on 2022/10/1.
 * <p>
 * description
 */

@Target(AnnotationTarget.FIELD)
@Retention(AnnotationRetention.BINARY)
annotation class AutelConverter(
    val canSet: Boolean = false,
    val canGet: Boolean = false,
    val canAction: Boolean = false,
    val canListen: Boolean = false,
    val paramConverter: String = "",
    val resultConverter: String = "",
    val paramBean: String = "",
    val paramMsg: String = "",
    val resultBea: String = "",
    val resultMsg: String = ""
)
