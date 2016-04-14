template <typename FunctionalityTn> class Daemon {
 public:
  typedef FunctionalityTn Functionality;
  typedef Daemon<Functionality> This;
  template <typename... Args> Init(Args args);
  int Start();
  
protected:
 Functionality const * worker() const {return worker_ptr;}
 Functionality       * worker()       {return worker_ptr;}
  
private:
  UniquePointer<Functionality> worker_ptr;
};
