package com.ccand99.apt_compiler

import com.ccand99.apt_annotations.AutelConverter
import com.google.auto.service.AutoService
import com.squareup.kotlinpoet.*
import com.squareup.kotlinpoet.ParameterizedTypeName.Companion.parameterizedBy
import java.io.IOException
import javax.annotation.processing.*
import javax.lang.model.SourceVersion
import javax.lang.model.element.Element
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
            var autelConvertElementInfos = elementInfoMap[enclosingElement]
//            if (autelConverterElements == null) {
//                autelConverterElements = ArrayList()
//                elementMap[enclosingElement] = autelConverterElements
//            }
            if (autelConvertElementInfos == null) {
                autelConvertElementInfos = ArrayList()
                elementInfoMap[enclosingElement] = autelConvertElementInfos
            }

//            autelConverterElements.add(element)
            val elementInfo = createElementInfo(element)
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

            autelConverterElements.forEach {

                println("forEach it: $it")

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
                val parameterized = ClassName(
                    "com.ccand99.apt",
                    it.resultBean
                )

                println("parameterized------------> $parameterized")
                val parameterizedAutelKeyInfoClass = autelKeyInfoClass.parameterizedBy(voidParameterized, parameterized)
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
                        "    "+it.keyName+",\n" +
                        "    AutelEmptyConvert(),\n" +
                        "    AutelEmptyConvert()\n" +
                        ")" +
                        if (it.canGet) ".canGet(true)" else "" +
                        if (it.canSet) ".canSet(true)" else "" +
                        if (it.canAction) ".canPerformAction(true)" else ""

                println("initCode------------> $initCode")
                // 类属性
                val keyProperty = PropertySpec.builder(it.keyName, parameterizedAutelKeyInfoClass)
                    .initializer(initCode)
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

    private fun createElementInfo(element: Element): AutelConvertInfo {
        val annotation = element.getAnnotation(AutelConverter::class.java)
        val convertInfo = AutelConvertInfo(keyName = annotation.keyName)
        convertInfo.element = element
        convertInfo.canSet = annotation.canSet
        convertInfo.canGet= annotation.canGet
        convertInfo.canAction= annotation.canAction
        convertInfo.canListen= annotation.canListen
        convertInfo.paramConverter= annotation.paramConverter
        convertInfo.resultConverter= annotation.resultConverter
        convertInfo.paramBean= annotation.paramBean
        convertInfo.paramMsg= annotation.paramMsg
        convertInfo.resultBean= annotation.resultBean
        convertInfo.resultMsg= annotation.resultMsg
        return convertInfo
    }


}