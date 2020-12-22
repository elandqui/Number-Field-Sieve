#include <NTL/ZZ.h>
#include <NTL/RR.h>
#include <math.h>

void setFactorBase(long FB1[], long FB2[][2], long len, long B, ZZ N);
void getSquareRoots(ZZ SR[], long FB[], long B, ZZ N);
void sieve(short SI[], long FB[], ZZ SR[], long B, long M);

// This will fill the factor base array with primes less than B and will change len
// to the length of the array.
void setFactorBase(long FB1[], long FB2[][2], long len, long B, ZZ N){
  long i=1;
  ZZ P = to_ZZ(1);
  FB1[0]= -1;
  while(i<len && P<B){
    P=NextPrime(P);
    FB1[i]=to_long(P);
    i++;
    P++;
  }
  
  // We may have overestimated the size of FB1.
  if(i<len){
  	FB1.setLength(i);
	len = i;
  }
  // Or we may have underestimated it.
  else{
  	while(P<B){
  		FB1.setLength(i);
  		P=NextPrime(P);
  		FB1[i] = P;
  		i++;
  		P++;
  	}
  	len = i;
  }

	// Now we work on building the second factor base.
	


}

// This will fill the array sqroots with numbers a such that:
// a=x-floor(sqrt(N)), and x^2 = N (mod p) for each p in the factor base
// Note that we don't include p=-1, since it is a unit and does not
// contribute any bits to the values of Q(x).

void getSquareRoots(ZZ sqroots[], long FB[], long B, ZZ N){
  long i=1;
  ZZ n = SqrRoot(N);
  if(FB[i]==to_ZZ(2)){
    sqroots[i] = to_ZZ(1);
    i++;
  }
  while(i<B){
    sqroots[i]=(SqrRootMod(N%FB[i], FB[i])-n)%FB[i];
    i++;
  }
}

// Let the sieving begin:
void sieve(short SI[], long FB[], ZZ SR[], long B, long M){
  // The sieving interval is [-M/2 + 1, M/2], so 0 is at position (M-1)/2.
  long a;
  long i;
  short bits;
  // For every relevant prime in the factor base.
  // Note: -1 is a factor of everything in the left half of the sieving interval.
  for(i=1; i<B; i++){
    bits = (short)NumBits(FB[i]);
    a = (M-1)/2 + SR[i];

    // Sieve to the right.
    while(a<B){
      SI[a]+=bits;
      a+=FB[i];
    } 

    a=(M-1)/2 + SR[i]-FB[i];

    // Then sieve to the left.
    while(a>=0){
      SI[a]+=bits;
      a+=FB[i];
    }
  }
}

int main(){

  // number to be factored, which is the 7th Fermat number
  ZZ N = pow(to_ZZ(2), 128) + to_ZZ(1);

  // The degree of the polynomial. Will be 3 for all N less than 80 digits 
  // and 5 otherwise
  int d = 3;

  // Bound for the size of the primes in both of the factor bases.
  // It will also represent the length of the sieving arrays: a's [-B, B] and b's [1, B]
  ZZ B = toLong(TruncToZZ(exp(.5*(pow(d*log((long)d)), 2)+pow(log(pow(to_RR(N), to_RR(1/((long)d)))*log(log(pow(to_RR(N), to_RR(1/((long)d)))+pow(d*log((long)d)), 2), to_RR(.5))))))));

  // Treshhold for sieving.
  int T;

  // The largest prime in the factor base.
  long p_max;

  long len = to_long(to_RR(N)/to_RR(log(N)));

  // The rational factor base.
  long FB1[len];

  // The algebraic factor base.
  long FB2[len][2]

  // The sieving interval. We will fix some b and sieve over these a's
  short SI[2*B];

  setFactorBase(FB1, FB2, &len, B, N);
  
  cout<<"Factor base complete.\n";

  getSquareRoots(SR, FB, B, N);

  //Now we do the sieving.
  sieve(SI, FB, SR, B, M);

  for(long i=0; i<M; i++)
    cout<<SI[i]<<"  ";

  p_max = FB[B-1];

  // Set the threshold.
  T= (int)to_long(TruncToZZ(log(to_RR(N))/2 + log(to_RR(M)) - 2*log(p_max)));
  
}
