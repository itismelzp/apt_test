package com.ccand99.tacos.web;

import org.springframework.stereotype.Controller;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;


@Controller
public class TestController {
    
    @RequestMapping("/test")
    public String test(){
        return "test";
    }
}
