package com.ccand99.domain.factory;

import com.ccand99.domain.User;
import org.springframework.beans.factory.DisposableBean;
import org.springframework.beans.factory.InitializingBean;

import javax.annotation.PostConstruct;
import javax.annotation.PreDestroy;

/**
 * User 工厂类
 */
public class UserFactoryImpl implements IUserFactory, InitializingBean, DisposableBean {

    //1. 初始化方法 基于 @PostConstruct 注解 优先度最高
    @PostConstruct
    public void init(){
        System.out.println("@PostConstruct UserFactory 初始化中");
    }

    //2. 初始化方法  优先度最低
    public void initUserFactory(){
        System.out.println("自定义 initUserFactory 初始化中");
    }

    public User createUser(){
        return new User("UserFactoryImpl");
    }

    //3. 初始化方法 InitializingBean接口 中等优先
    @Override
    public void afterPropertiesSet() throws Exception {
        System.out.println("InitializingBean方式 afterPropertiesSet 初始化中");
    }

    @PreDestroy
    public void preDestroy() {
        System.out.println("@PreDestroy 销毁中");
    }

    @Override
    public void destroy() throws Exception {
        System.out.println("DisposableBean接口 销毁中");
    }

    public void doDestroy() throws Exception {
        System.out.println("自定义方法 销毁中");
    }

    @Override
    protected void finalize() throws Throwable {
        System.out.println("当前对象被gc");
        super.finalize();
    }
}
