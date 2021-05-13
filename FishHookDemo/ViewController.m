//
//  ViewController.m
//  FishHookDemo
//
//  Created by ZP on 2021/4/30.
//

#import "ViewController.h"
#import "fishhook.h"
#include <mach-o/dyld.h>

@interface ViewController ()

@end

@implementation ViewController


//void func(const char * str) {
//    NSLog(@"%s",str);
//}

- (void)viewDidLoad {
    [super viewDidLoad];
//    NSLog(@"外部函数第二次调用");
//    NSLog(@"before hook");
    [self hook_NSLog];
//    [self hook_func];
//    [self hook_Binder];
//    NSLog(@"after hook");
}

- (void)touchesBegan:(NSSet<UITouch *> *)touches withEvent:(UIEvent *)event {
//    test();
//    [self test];
    NSLog(@"after hook");
}

//- (void)test {
//    [self test1];
//}
//- (void)test1 {
//    NSLog(@"外部函数第一次调用");
//}


//void test() {
//    test1();
//}
//
//void test1() {
//}

//- (void)viewDidLoad {
//    [super viewDidLoad];
//    NSLog(@"外部函数第一次调用");
//    NSLog(@"外部函数第二次调用");
////    NSLog(@"before");
////    [self hook_NSLog];
////    NSLog(@"after");
////    [self hook_func]
////    func("HotPotCat");
//
//    //bl  本地代码地址 （rebase + ASLR）
//}

- (void)hook_func {
    struct rebinding rebindFunc;
    rebindFunc.name = "func";
    rebindFunc.replacement = HP_func;
    rebindFunc.replaced = (void *)&original_func;

    struct rebinding rebinds[] = {rebindFunc};

    rebind_symbols(rebinds, 1);
}

//原函数，函数指针
static void (*original_func)(const char * str);

//新函数
void HP_func(const char * str) {
    NSLog(@"Hook func");
    original_func(str);
}


- (void)hook_NSLog {
    struct rebinding rebindNSLog;
    rebindNSLog.name = "NSLog";
    rebindNSLog.replacement = HP_NSLog;
    rebindNSLog.replaced = (void *)&sys_NSLog;

    struct rebinding rebinds[] = {rebindNSLog};

    rebind_symbols(rebinds, 1);
}

//原函数，函数指针
static void (*sys_NSLog)(NSString *format, ...);

//新函数
void HP_NSLog(NSString *format, ...) {
    format = [format stringByAppendingFormat:@"\n Hook"];
    //调用系统NSLog
    sys_NSLog(format);
}

- (void)hook_Binder {
    struct rebinding binder;
    binder.name = "dyld_stub_binder";
    binder.replacement = HP_Binder;
    binder.replaced = (void *)&sys_Binder;

    struct rebinding rebinds[] = {binder};

    rebind_symbols(rebinds, 1);
}



//原函数，函数指针
static void (*sys_Binder)(void) __asm__("dyld_stub_binder");


//新函数
void HP_Binder(void) {
    sys_Binder();
}


@end
