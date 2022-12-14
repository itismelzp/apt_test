package com.ccand99.kotlininspring

import org.eclipse.swt.SWT
import org.eclipse.swt.browser.Browser
import org.eclipse.swt.widgets.Display
import org.eclipse.swt.widgets.Shell
import org.springframework.boot.autoconfigure.SpringBootApplication
import org.springframework.boot.runApplication

@SpringBootApplication
class KotlininspringApplication

fun main(args: Array<String>) {
    runApplication<KotlininspringApplication>(*args)
    val display = Display()
    val shell = Shell(display)
    shell.text = "brows"
    shell.setSize(1920,1080)
    val browser = Browser(shell, SWT.ALL)
    browser.setBounds(0,0,1920,1080)
    browser.url = "http://www.baidu.com"
    shell.open()
    while (!shell.isDisposed){
        if (!display.readAndDispatch()){
            display.sleep()
        }
    }
    display.dispose()
}
