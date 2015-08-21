
#ifndef lifetimeCalcLAPD_h
#define lifetimeCalcLAPD_h

class lifetimeCalcLAPD{
  public:
    void CalculateLifetime(TNtuple* oscillData, int IPrM);
    double CatPeak(){ return fCatPeak;}
    double CatTime(){ return fCatTime;}
    double CatBase(){ return fCatBase;}
    double AnoPeak(){ return fAnoPeak;}
    double AnoTime(){ return fAnoTime;}
    double AnoBase(){ return fAnoBase;}
    double AnoRise(){ return fAnoRise;}
    double CathF(){ return fCathF;}
    double AnoF(){ return fAnoF;}
    double CatRMS(){ return fCatRMS;}
    double AnoRMS(){ return fAnoRMS;}
    double AnoTrue(){ return fAnoTrue;}
    double CatTrue(){ return fCatTrue;}
    double Lifetime(){ return fLifeTime;}
    double Impurities(){ return fImpurities;}

  private:
    double fCatPeak;
    double fCatTime;
    double fCatBase;
    double fAnoPeak;
    double fAnoTime;
    double fAnoBase;
    double fAnoRise;
    double fCathF;
    double fAnoF;
    double fCatRMS;
    double fAnoRMS;
    double fAnoTrue;
    double fCatTrue;
    double fLifeTime;
    double fImpurities;

};

#endif
