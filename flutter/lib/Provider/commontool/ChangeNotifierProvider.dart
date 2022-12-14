//订阅者
import 'package:first_flutter_app/Provider/commontool/InheritedProvider.dart';
import 'package:flutter/cupertino.dart';

//Type _typeOf<T>() => T;

class ChangeNotifierProvider<T extends ChangeNotifier>
    extends StatefulWidget {
  ChangeNotifierProvider({
    Key key,
    this.data,
    this.child,
  });

  final Widget child;
  final T data;

  //定义一个便捷方法，方便子树中的Widget获取共享数据
  static T of<T>(BuildContext context, {bool listen = true}) {
    //final type = _typeOf<InheritedProvide<T>>();
    /*final provider = listen ? context.inheritFromWidgetOfExactType(type) as InheritedProvider<T>
        :context.ancestorInheritedElementForWidgetOfExactType(type)?.widget as InheritedProvider<T> ;*/
    final provider = listen ?
    context.dependOnInheritedWidgetOfExactType<InheritedProvider<T>>() :
    context.getElementForInheritedWidgetOfExactType<InheritedProvider<T>>()?.widget as InheritedProvider<T>;

    return provider.data;
  }

  @override
  State<StatefulWidget> createState() => _ChangeNotifierProviderState<T>();
}

class _ChangeNotifierProviderState<T extends ChangeNotifier>
    extends State<ChangeNotifierProvider<T>> {
  void update() {
    //如果数据发生变化（model 类调用了notifyListeners）,则重新构建 InheritedProvider
    setState(() {});
  }

  @override
  void didUpdateWidget(ChangeNotifierProvider<T> oldWidget) {
    //当Provider更新时，如果新旧数据不"=="，则解绑旧数据监听，同时添加新数据监听
    if (widget.data != oldWidget.data) {
      oldWidget.data.removeListener(update);
      widget.data.addListener(update);
    }
    super.didUpdateWidget(oldWidget);
  }

  @override
  void initState() {
    // 为model添加监听
    widget.data.addListener(update);
    super.initState();
  }

  @override
  void dispose() {
    // 移除model的监听器
    widget.data.removeListener(update);
    super.dispose();
  }

  @override
  Widget build(BuildContext context) {
    return InheritedProvider<T>(
      data: widget.data,
      child: widget.child,
    );
  }
}
