package com.ccand99.apt_compiler

import com.ccand99.apt_annotations.AutelConverter
import com.google.auto.service.AutoService
import com.squareup.kotlinpoet.*
import com.squareup.kotlinpoet.ParameterizedTypeName.Companion.parameterizedBy
import java.io.IOException
import javax.annotation.processing.*
import javax.lang.model.SourceVersion
import javax.lang.model.element.Element
import javax.lang.model.element.ExecutableElement
import javax.lang.model.element.TypeElement
import javax.lang.model.util.Elements

@AutoService(Processor::class)
class AutelConverterProcessor : AbstractProcessor() {

    private var mFiler: Filer? = null

    private var mElementUtils: Elements? = null

    override fun init(processingEnv: ProcessingEnvironment?) {
        super.init(processingEnv)
        mFiler = processingEnv?.filer
        mElementUtils = processingEnv?.elementUtils
    }

    // 指定处理的版本
    override fun getSupportedSourceVersion(): SourceVersion {
        return SourceVersion.latestSupported()
    }

    // 给到需要处理的注解
    override fun getSupportedAnnotationTypes(): MutableSet<String> {
        val types: LinkedHashSet<String> = LinkedHashSet()
        getSupportedAnnotations().forEach { clazz: Class<out Annotation> ->
            types.add(clazz.canonicalName)
        }
        return types
    }

    private fun getSupportedAnnotations(): Set<Class<out Annotation>> {
        val annotations: LinkedHashSet<Class<out Annotation>> = LinkedHashSet()
        // 需要解析的自定义注解
        annotations.add(AutelConverter::class.java)
        return annotations
    }

    override fun process(
        p0: MutableSet<out TypeElement>?,
        roundEnvironment: RoundEnvironment?
    ): Boolean {
        //解析属性 activity ->list<Element>
//        val elementMap = LinkedHashMap<Element, ArrayList<Element>>()
        val elementInfoMap = LinkedHashMap<Element, ArrayList<AutelConvertInfo>>()

        // 有注解就会进来
        roundEnvironment?.getElementsAnnotatedWith(AutelConverter::class.java)?.forEach { element ->
            //注解属性名称
            println("element.simpleName ------------> ${element.simpleName}")
            //注解所在类名称
            println("element.enclosingElement.simpleName ------------> ${element.enclosingElement.simpleName}")
            //按照类整理属性
            val enclosingElement = element.enclosingElement
//            var autelConverterElements = elementMap[enclosingElement]
            println("element.enclosingElement ------------> ${element.enclosingElement}")
            var autelConvertElementInfos = elementInfoMap[enclosingElement]
//            if (autelConverterElements == null) {
//                autelConverterElements = ArrayList()
//                elementMap[enclosingElement] = autelConverterElements
//            }
            println("autelConvertElementInfos ------------> $autelConvertElementInfos")
            if (autelConvertElementInfos == null) {
                autelConvertElementInfos = ArrayList()
                elementInfoMap[enclosingElement] = autelConvertElementInfos
            }
//            autelConverterElements.add(element)
            println("elementInfo ------------> begin")
            val elementInfo = createElementInfo(element)

            println("elementInfo ------------> $elementInfo")
            autelConvertElementInfos.add(elementInfo)
        }

        // 生成代码
        elementInfoMap.entries.forEach { it ->

            val clazz = it.key
            val autelConverterElements = it.value
            println("clazz------------> ${clazz.simpleName}")
            // 动态获取包名
            val packageName = mElementUtils?.getPackageOf(clazz)?.qualifiedName?.toString()
                ?: throw RuntimeException("无法获取包名")
            val messageKeyStr = clazz.simpleName.toString()
            val messageKeyClass = ClassName(packageName, messageKeyStr)

            println("packageName------------> $packageName")
            // component属性
            val componentClass = ClassName(
                "com.ccand99.apt",
                "ComponentType"
            )
            println("componentClass------------> $componentClass")
            // 构造类
            // public final 类kotlin为KModifier
            val clazzBuilder = TypeSpec.objectBuilder("WaypointMissionKey")

            val componentProperty = PropertySpec.builder("component", componentClass)
                .initializer("ComponentType.MISSION")
                .build()

            clazzBuilder.addProperty(componentProperty)

            autelConverterElements.forEach { info ->

                println("forEach it: $info")

                // autelKey属性
                val autelKeyInfoClass = ClassName(
                    "com.autel.drone.sdk.vmodelx.manager.keyvalue.key",
                    "AutelActionKeyInfo")

                // messageTypeClass
//                val messageTypeClass = ClassName(
//                    "com.autel.drone.sdk.pbprotocol.interaction.constants",
//                    "MessageTypeConstant")

                val voidParameterized = ClassName(
                    "java.lang", "Void"
                )
                println("voidParameterized------------> $voidParameterized")

                val classFromAnnotation = getClassFromAnnotation(info.element)
//                val parameterized = ClassName("com.ccand99.apt", it.resultBean)
                val parameterized = ClassName("com.ccand99.apt", classFromAnnotation?.let {
                    classFromAnnotation.substring(classFromAnnotation.lastIndexOf("."), classFromAnnotation.length)
                }?:"Void")
                println("parameterized------------> $parameterized")

                val parameterizedAutelKeyInfoClass = autelKeyInfoClass.parameterizedBy(voidParameterized, parameterized)
                println("parameterizedAutelKeyInfoClass------------> $parameterizedAutelKeyInfoClass")
                // 类属性
                val initStr = """AutelActionKeyInfo(
                    |component.value,
                    |MessageTypeConstant.MISSION_WAYPOINT_ENTER_MSG,
                    |AutelEmptyConvert(),
                    |AutelEmptyConvert())
                    |.canPerformAction(true)
                    """.trimMargin()

                val initCode = "AutelActionKeyInfo(\n" +
                        "    component.value,\n" +
//                        "    "+it.keyName+",\n" +
                        "    MessageTypeConstant.MISSION_WAYPOINT_ENTER_MSG,\n" +
                        "    AutelEmptyConvert(),\n" +
                        "    AutelEmptyConvert()\n" +
                        ")" +
                        (if (info.canGet) ".canGet(true)" else "") +
                        (if (info.canSet) ".canSet(true)" else "") +
                        (if (info.canAction) ".canPerformAction(true)" else "")

                println("initCode------------> $initCode")
                // 类属性
                val keyProperty = PropertySpec.builder(info.keyName, parameterizedAutelKeyInfoClass)
                    .initializer(initCode)

                println("keyProperty------------> $keyProperty")
                clazzBuilder.addProperty(keyProperty.build())
            }

            //生成类文件
            val classFile = FileSpec.builder(packageName, "WaypointMissionKey")
                .addType(clazzBuilder.build())
                .addComment("generated by IDE, do not edit it.")
                .build()

            println("classFile------------>\n$classFile")
            //classFile.writeTo(System.out)
            //输出的文件映射
            try {
                mFiler?.let { filer -> classFile.writeTo(filer) }
            } catch (e: IOException) {
                println(e.message)
            }

        }

        return false
    }

    private fun getClassFromAnnotation(key: Element): String? {

        println("getClassFromAnnotation------------> key: $key")
        val annotationMirrors = key.annotationMirrors
        for (annotationMirror in annotationMirrors) {
            println("getClassFromAnnotation------------> annotationMirror: $key")
            println("getClassFromAnnotation------------> annotationType: ${annotationMirror.annotationType}")
            if (AutelConverter::class.java.name == annotationMirror.annotationType.toString()) {
                val keySet: Set<ExecutableElement> = annotationMirror.elementValues.keys
                for (executableElement in keySet) {
                    if (executableElement.simpleName.toString() == "paramBean") {
                        return annotationMirror.elementValues[executableElement]!!.value.toString()
                    }
                }
            }
        }
        return null
    }

    private fun createElementInfo(element: Element): AutelConvertInfo {
        val annotation = element.getAnnotation(AutelConverter::class.java)
        println("createElementInfo ------------> 0")
        println("createElementInfo ------------> $element")
        println("createElementInfo ------------> $annotation")

        val convertInfo = AutelConvertInfo(keyName = annotation.keyName, element = element)
        println("createElementInfo ------------> 1")
        convertInfo.element = element
        convertInfo.canSet = annotation.canSet
        convertInfo.canGet= annotation.canGet
        convertInfo.canAction= annotation.canAction
        println("createElementInfo ------------> 2")
        convertInfo.canListen = annotation.canListen
//        convertInfo.paramBean = annotation.paramBean
        convertInfo.paramConverter= annotation.paramConverter
        convertInfo.resultConverter= annotation.resultConverter
        println("createElementInfo ------------> 3")

        convertInfo.paramMsg= annotation.paramMsg
        convertInfo.resultMsg= annotation.resultMsg
        return convertInfo
    }


}