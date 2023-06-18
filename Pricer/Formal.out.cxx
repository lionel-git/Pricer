// ========= GENERATED CODE =========== 
// Sun Jun 18 11:43:32 2023

{
size_t i = 1;
double c1a = edp_coeffs_[i].c1a_;
double c1b = edp_coeffs_[i].c1b_;
double c1c = edp_coeffs_[i].c1c_;
double c2a = edp_coeffs_[i].c2a_;
double c2b = edp_coeffs_[i].c2b_;
double c2c = edp_coeffs_[i].c2c_;
t[i] = - (((1-theta_)*(V[i]-dt*(((-r)*x_[i]*c1a*U_down+r*V[i])+(-0.5)*vol_bs_*vol_bs_*x_[i]*x_[i]*c2a*U_down))+theta_*(V[i]-dt*((r*V[i]+(-r)*x_[i]*((c1a*V[i-1]+c1b*V[i])+c1c*V[i+1]))+(-0.5)*vol_bs_*vol_bs_*x_[i]*x_[i]*((c2a*V[i-1]+c2b*V[i])+c2c*V[i+1])))));
a[i] = 0;
b[i] = ((1-theta_)*(-dt*((-r)*x_[i]*c1b+(-0.5)*vol_bs_*vol_bs_*x_[i]*x_[i]*c2b))-1);
c[i] = (1-theta_)*(-dt*((-r)*x_[i]*c1c+(-0.5)*vol_bs_*vol_bs_*x_[i]*x_[i]*c2c));
}
{
for (size_t i=2 ; i <= N-3 ; ++i) {
double c1a = edp_coeffs_[i].c1a_;
double c1b = edp_coeffs_[i].c1b_;
double c1c = edp_coeffs_[i].c1c_;
double c2a = edp_coeffs_[i].c2a_;
double c2b = edp_coeffs_[i].c2b_;
double c2c = edp_coeffs_[i].c2c_;
t[i] = - (((1-theta_)*(V[i]-dt*r*V[i])+theta_*(V[i]-dt*((r*V[i]+(-r)*x_[i]*((c1a*V[i-1]+c1b*V[i])+c1c*V[i+1]))+(-0.5)*vol_bs_*vol_bs_*x_[i]*x_[i]*((c2a*V[i-1]+c2b*V[i])+c2c*V[i+1])))));
a[i] = (1-theta_)*(-dt*((-r)*x_[i]*c1a+(-0.5)*vol_bs_*vol_bs_*x_[i]*x_[i]*c2a));
b[i] = ((1-theta_)*(-dt*((-r)*x_[i]*c1b+(-0.5)*vol_bs_*vol_bs_*x_[i]*x_[i]*c2b))-1);
c[i] = (1-theta_)*(-dt*((-r)*x_[i]*c1c+(-0.5)*vol_bs_*vol_bs_*x_[i]*x_[i]*c2c));
}
}
{
size_t i = N-2;
double c1a = edp_coeffs_[i].c1a_;
double c1b = edp_coeffs_[i].c1b_;
double c1c = edp_coeffs_[i].c1c_;
double c2a = edp_coeffs_[i].c2a_;
double c2b = edp_coeffs_[i].c2b_;
double c2c = edp_coeffs_[i].c2c_;
t[i] = - (((1-theta_)*(V[i]-dt*(((-r)*x_[i]*c1c*U_up+r*V[i])+(-0.5)*vol_bs_*vol_bs_*x_[i]*x_[i]*c2c*U_up))+theta_*(V[i]-dt*((r*V[i]+(-r)*x_[i]*((c1a*V[i-1]+c1b*V[i])+c1c*V[i+1]))+(-0.5)*vol_bs_*vol_bs_*x_[i]*x_[i]*((c2a*V[i-1]+c2b*V[i])+c2c*V[i+1])))));
a[i] = (1-theta_)*(-dt*((-r)*x_[i]*c1a+(-0.5)*vol_bs_*vol_bs_*x_[i]*x_[i]*c2a));
b[i] = ((1-theta_)*(-dt*((-r)*x_[i]*c1b+(-0.5)*vol_bs_*vol_bs_*x_[i]*x_[i]*c2b))-1);
c[i] = 0;
}
// ========= END GENERATED CODE ===========
