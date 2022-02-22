#ifndef __NON_COPY_ABLE__
#define __NON_COPY_ABLE__
class noncopyable {
 protected:
  noncopyable() {}
  ~noncopyable() {}

// 将类的复制构造函数和赋值函数声明为private，以禁止复制
 private:  
  noncopyable(const noncopyable&);
  const noncopyable& operator=(const noncopyable&);
};

#endif