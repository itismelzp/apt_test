package com.ccand99.apt_annotations

import java.lang.annotation.Retention
import java.lang.annotation.RetentionPolicy
import kotlin.reflect.KClass

@Retention(RetentionPolicy.CLASS)
@Target(AnnotationTarget.FIELD)
annotation class BindButton(val resId: Int,
                            val clazz: KClass<*>,
                            val flag: Int = -1)