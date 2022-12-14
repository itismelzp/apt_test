package mediator;

public class Development implements Department{

    private Mediator m;

    public Development(Mediator m) {
        this.m = m;
        m.register("development",this);
    }

    @Override
    public void selfAction() {
        System.out.println("研发中...");
    }

    @Override
    public void outAction() {
        System.out.println("申请研发资金");
        //调用同事方法时通过中介者
        m.command("finacial");
    }
}
