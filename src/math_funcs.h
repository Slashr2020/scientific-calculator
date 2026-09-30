#ifndef MATH_FUNCS
#define MATH_FUNCS

#define LN_2 0.693147180559945309417

static inline double sumd(double *a, int n){
  double res=0.0;
  for (int i=0; i<n; i++){
    res+=a[i];
  }
  return res;
}

static inline double ipowd(double a, int n){
  double res=1;
  for (int i=0; i<n; i++){
    res*=a;
  }
  return res;
}

static inline double ln(double n){
  int p = 0;
  if (n>=1.0){
    while (n>=1){
      n/=2.0;
      p++;
    }
  }
  else if (n<0.5){
    while (n<0.5){
      n*=2.0;
      p--;
    }
  }
  double x = (n-1)/(n+1);
  double res = 0.0;
  for (int i=1; i<64; i+=2){
    res+=ipowd(x,i)/i;
  }
  res*=2.0;
  return res+(double)p * LN_2;
}

static inline double exponent(double y){
  double res=1.0;
  double term=1.0;

  for (int k=1; k<32; k++){
    term*=y/k;
    res+=term;
  } return res;
}

static inline double powd(double x, double a){
  return exponent(a*ln(x));
}

#endif
