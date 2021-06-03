#include        "Gidra-16.h"

#include        <math.h>

/*------------- Интерполяционные   таблицы -----------------------------*/
//         Температура воды на входе в канал, C
static  double  const   t_l[12] = {
        10, 20, 50, 90, 100, 110, 200,
        250, 260, 270, 280, 290 };
//         Энтальпия недогретой воды, ккал/кг
static  double  const   i_t[12] = {
        11.6,  21.6, 51.4, 91.3, 101.3, 111.4,
        204.1, 259.3, 271., 283., 295.3, 308.1
};
//         Удельный   об"ем,  м**3/кг
static  double  const   v_t[12] = {
        0.9972e-3, 0.9988e-3 , 0.1009e-2,
        0.10325e-2, 0.10399e-2, 0.1048e-2,
        0.11513e-2, 0.12459e-2, 0.12708e-2,
        0.12988e-2, 0.13307e-2, 0.13653e-2
};
//        Динамическая  вязкость н.в.,  кг*с/м**2
static  double  const   mu_t[12] = {
        1.33e-3, 0.102e-3, 0.562e-4, 0.323e-4,
        0.29e-4, 0.266e-4, 0.141e-4, 0.113e-4,
        0.109e-4, 0.105e-4, 0.101e-4, 0.97e-5
};
//       Давления,    кг/см**2
static  double  const   p_t[12] = {
        12.8, 15.9, 19.5, 23.7, 28.5, 34.1,
        40.6, 47.9, 56.1, 65.5, 75.9, 87.6
};
//        Динамическая  вязкость на линии насыщения,  кг*с/м**2
static  double  const   mu1_t[12] = {
        0.148e-4, 0.141e-4, 0.134e-4,
        0.128e-4, 0.122e-4, 0.117e-4,
        0.112e-4, 0.108e-4, 0.104e-4,
        1.e-5,  0.96e-5, 0.93e-5
};


double  i1, i2, v1, v2, v3, gamma1, gamma2, r;

double  strt_boil; // точка начала кипения
double  Param_A = 1.36e+11; /* Переходной коэфф. от мощности к потоку нейтронов,
                               1/(Вт*c*кг) */
int     m_az = 50; /* число расчетных точек на АЗ,первая точка находится
                      на краю активной зоны, а последняя внутри АЗ,
                      первая точка имеет номер 0,а последняя (m_az-1) */
int     m_pvk = 50; /* число расчетных точек на ПВК, первая точка находится
                       на границе АЗ, а последняя на конце ПВК,
                       первая точка имеет номер m_az, а последняя m_az+m_pvk-1 */
double  et;
double  PNa;


double  F[71];     //  pаспpеделение плотности потока нейтpонов по высоте АЗ
double  Gamma[119];//  плотность смеси, кг/м**3
double  Gt[119];   //  об'емное паpосодеpжание, %
double  Ze[119];   //  кооpдинаты сеpедин pасчетных участков на уpовне АЗ
double  Ts[119];   //  темпеpатуpа насыщения, гp.С
double  S[119];    //  пpодное сечение, м**2
double  OM1[119];  //  лямбда азота/ скорость жидкой фазы
double  OM2[119];  //  лямбда азота/ скорость паровой фазы
double  P[119];    //  давлeние, кг/м**2
double  SR0[119];  //  произведение сечения на плотность недогретой воды
double  SR01[119]; //  произведение сечения на плотность воды   на линии насыщения
double  QONT[119]; //  комплекс скорости обмена между фазами
double  OM[119];   //
double  Na[102];   //  распределение концентрации азота, 1/м
double  Naone[102];//  распределение концентрации азота, 1/м
double  q1one[102];
double  q1[102];
double  q2[102];
double  CDe[102];  // координата расчетного участка
double  POM[119];
double  PGamma[119];
double  dn[102];
double  dq[102];
double  pq1one[102];
double  PNaone[102];
double  POMZL[102];// распределение скорости смеси по АЗ и ПВК, м/c
double  PSS[102];  // распределение плотности смеси по АЗ и ПВК, кг/м**3
double  PD[102];   // распределение давления по АЗ и ПВК, кг/см**2
double  GtL[102];  // распределение объемного паросодержания по АЗ и ПВК, ед.



double  Get_F(int i) { return F[i]; }
double  Get_Gamma(int i) { return Gamma[i]; }
double  Get_Gt(int i) { return Gt[i]; }
double  Get_Ze(int i) { return Ze[i]; }
double  Get_Ts(int i) { return Ts[i]; }
double  Get_S(int i) { return S[i]; }
double  Get_OM1(int i) { return OM1[i]; }
double  Get_OM2(int i) { return OM2[i]; }
double  Get_P(int i) { return P[i]; }
double  Get_SR0(int i) { return SR0[i]; }
double  Get_SR01(int i) { return SR01[i]; }
double  Get_QONT(int i) { return QONT[i]; }
double  Get_OM(int i) { return OM[i]; }
double  Get_strt_boil(void) { return strt_boil; }
double  Get_Na(int i) { return Na[i]; }
double  Get_Naone(int i) { return Naone[i]; }
double  Get_q1one(int i) { return q1one[i]; }
double  Get_q1(int i) { return q1[i]; }
double  Get_q2(int i) { return q2[i]; }
double  Get_CDe(int i) { return CDe[i]; }
double  Get_Param_A(void) { return Param_A; }
double  Get_POMZL(int i) { return POMZL[i]; }
double  Get_PSS(int i) { return PSS[i]; }
double  Get_PD(int i) { return PD[i]; }
double  Get_GtL(int i) { return GtL[i]; }
void    Set_Param_A(double i) { Param_A = i; }
double  Get_m_az(void) { return m_az; }
void    Set_m_az(double i) { m_az = i; }
double  Get_m_pvk(void) { return m_pvk; }
void    Set_m_pvk(double i) { m_pvk = i; }
double  Get_dq(int i) { return dq[i]; }
double  Get_dn(int i) { return dn[i]; }




/*      Функция li1 для одномеpной линейной интеpполяции  по             */
/*            таблично заданным функциям и аpгументам                    */
/*                                                                       */
/*                 Лобков Боpис (МИФИ)                                   */
/*                  17-Янв-91                                            */
/*                                                                       */

double li1(int Max_Index, const double Arg_X[], const double Arg_Y[], double x)
{
    int  i = 0;
    if (x < Arg_X[0])
        return Arg_Y[0];
    if (x > Arg_X[Max_Index - 1])
        return Arg_Y[Max_Index - 1];
    while (x >= Arg_X[++i]);
    return (Arg_Y[i - 1] +
        (Arg_Y[i] - Arg_Y[i - 1]) * (x - Arg_X[i - 1]) / (Arg_X[i] - Arg_X[i - 1]));
}



void spl(double ps, double* ptr_ts, double* ptr_xi1, double* ptr_v1,
    double* ptr_ga1, double* ptr_xi2, double* ptr_v2, double* ptr_ga2,
    double* ptr_r)
{
    double x;
    x = log(ps * 0.0001);
    //  *ptr_ts - температура насыщения, град.С
    *ptr_ts = ((((((((((-4.29246029e-8 * x - 4.26856851e-7) * x
        + 1.53437313e-6) * x + 2.20717118e-5) * x - 1.74177519e-5) * x
        - 3.73934843e-4) * x + 1.32837729e-3) + 2.12968201e-2) * x
        + 2.10778046e-1) * x + 2.37535765) * x + 27.8542422) * x + 99.0927120;
    x = *ptr_ts / 100.;
    //  *ptr_xi1 - энтальпия воды на линии насыщения,  ккал/кг
    *ptr_xi1 = (((((((((1.1936422e-4 * x - 1.699491e-3) * x
        + 1.0322371e-2) * x - 3.453679e-2) * x + 6.921469e-2) * x
        - 8.382342e-2) * x + 6.222956e-2) * x - 2.5507948e-2) * x
        + 4.2290733e-1) * x - 4.16e-5) / 4.18887e-3;
    //  *ptr_v1 - удельный об"ем воды,   м**3/кг
    *ptr_v1 = ((((((((5.968357e-7 * x - 8.267041e-6) * x + 4.8021276e-5) * x
        - 1.5042234e-4) * x + 2.7254276e-4) * x - 2.7979589e-4) * x
        + 1.4001053e-4) * x + 2.0639347e-5) * x + 1.390533e-7) * x + 1.0001789e-3;
    *ptr_ga1 = 1. / (*ptr_v1);  /*удельный вес, кг/м**3  */
//  *ptr_xi2 - энтальпия   пара,   ккал/кг
    *ptr_xi2 = (((((((-3.4714286e-4 * x + 2.7325329e-3) * x
        - 8.0477568e-3) * x + 9.38497e-3) * x - 1.2737746e-2) * x +
        1.95911e-4) * x + 1.843054e-1) * x + 2.50096) / 4.18887e-3;
    //  *ptr_v2 - удельный об'ем пара,   м**3/кг
    *ptr_v2 = ((((((((2.0409848e-6 * x - 2.8638253e-5) * x + 1.4519652e-4) * x
        - 3.4194103e-4) * x + 3.3979946e-4) * x - 3.168401e-4) * x - 3.1913345e-5)
        * x + 4.5945234e-3) * x + 1.26e-2) / (ps * 9.81e-7);
    *ptr_ga2 = 1. / (*ptr_v2); /*удельный вес, кг/м**3  */
//  *ptr_r - теплота парообразования,    ккал/кг
    *ptr_r = *ptr_xi2 - *ptr_xi1;
}


void one_phase(double i, double G, double d, double s, double* ptr_psi_m,
    double* ptr_psi_t, double* ptr_R, double* ptr_fi, double* ptr_Re,
    double* ptr_gamma, double* ptr_om1, double* ptr_om)
{
    double  mu;
    double  v;

    mu = li1(12, i_t, mu_t, i);
    v = li1(12, i_t, v_t, i);
    *ptr_gamma = 1. / v;
    *ptr_psi_m = *ptr_psi_t = 1. / ((*ptr_gamma) * v1);
    *ptr_R = 1.;
    *ptr_fi = 0.;
    *ptr_Re = G * d / (3600. * s * mu * grav);
    *ptr_om1 = (G * v) / (3600.0 * s);

    *ptr_om = (G * v) / (3600.0 * s);

}

void two_phase(double p, double x, double nu1, double G, double d,
    double s, double* ptr_psi_m, double* ptr_psi_t, double* ptr_R,
    double* ptr_fi, double* ptr_Re, double* ptr_gamma, double* ptr_om1,
    double* ptr_om2, double* ptr_om)
{
    double w0;
    double temp, K_pr;
    double beta, Fr;
    if (x >= 1.)
        x = 0.999;
    *ptr_psi_m = 1.;
    w0 = G * v1 / (3600. * s);
    *ptr_psi_t = 1. + 0.57 * sqrt(sqrt(sqrt(x))) * (1. - x) * (1. - x) *
        ((1. / (0.2 + w0 * v1 / (v2 * sqrt(grav * d)))) - 5.2 * x * x);
    *ptr_R = 1. + x * (v2 / v1 - 1.);
    Fr = w0 * w0 / (grav * d);
    temp = (1. - x) * v1 / (x * v2);
    beta = 1. / (1. + temp);
    K_pr = 1. + (0.6 + 1.5 * beta * beta) * (1. - p / P_kr) /
        sqrt(sqrt(Fr));
    *ptr_fi = 1. / (1. + temp * K_pr);
    *ptr_Re = w0 * d / nu1;
    *ptr_gamma = gamma1 * (1. - *ptr_fi) + gamma2 * (*ptr_fi);
    *ptr_om1 = G / (3600.0 * s * ((1 - *ptr_fi) / v1 + (*ptr_fi * K_pr) / v2));
    *ptr_om2 = K_pr * (*ptr_om1);
    *ptr_om = G / (3600.0 * s * (*ptr_gamma));
}


void gidra(double G, double N, double p0, double t0, double Lpvk)
/*---------------------- Паpаметpы функции gidra   ------------------------*/
/*                  Входы:                                                 */
/*    G  - pаcход т.н. чеpез канал, м**3/ч                                 */
/*    N  - мощность канала, МВт                                            */
/*    p0 - давление в баpабан-сепаpатоpе, кг/см**2                         */
/*    t0 - температура на входе, C                                         */
/*    Lpvk - длина ПВК, м                                                  */
/*-------------------------------------------------------------------------*/
{
    // Длины участков в отводящей зоне,м
    static  double  len_pvk[23] = {
            0.4, 1.5, 1.38, 7., 0.4, 2.3, 1.5, 1., 1.5, 1.5, 1., 1.75,
            1., 0.47, 1., 0.12, 0.8, 1., 0.24, 2., 0.4, 0.512, 0.148
    };
    // Перепады  высот,м
    static  const   double  h_pvk[23] = {
            0.17, 1.3, 2.38, 3., 0., 0., 0., 0., 0., 0., 0., 0., 0., 0.465,
            1., 0.12, 0.835, 1., 0.24, 2., 0.4, 0.512, 0.148
    };
    // Гидр. диаметры,м
    static  const   double  d_pvk[23] = {
            0.068, 0.068, 0.068, 0.068, 0.068, 0.068, 0.068, 0.068, 0.068, 0.068,
            0.068, 0.068, 0.068, 0.042, 0.042, 0.039, 0.04, 0.0207, 0.043, 0.049,
            0.049, 0.047, 0.008562
    };
    // Проходные   сечения,м**2
    static  const   double  s_pvk[23] = {
            0.003632, 0.003632, 0.003632, 0.003632,
            0.003632, 0.003632, 0.003632, 0.003632,
            0.003632, 0.003632, 0.003632, 0.003632,
            0.003632, 0.007323, 0.007323, 0.003951,
            0.004084, 0.00232 , 0.004289, 0.004657,
            0.004657, 0.004171, 0.002273
    };
    //  Коэф. местного сопротивления
    static  const   double  csi_pvk[23] = {
            1.2, 0.0635, 0.127, 0., 0.4, 0.05,
            0.115, 0.0515, 0.0892, 0.0892, 0.0892,
            0.   , 0.0858, 2.19,   0.0835, 1.6,
            0.1, 0.5, 0.046, 0., 0.087, 0.1, 0.963
    };
    //  Шероховатости
    static  const   double  sh_pvk[23] = {
            0.25e-4, 0.25e-4, 0.25e-4, 0.25e-4,
            0.25e-4, 0.25e-4, 0.25e-4, 0.25e-4,
            0.25e-4, 0.25e-4, 0.25e-4, 0.25e-4,
            0.25e-4,   1.e-5,   1.e-5,   1.e-5,
            1.e-5,     1.e-5,   1.e-5,   1.e-5,
            1.e-5,     1.e-5,   1.e-5
    };

    /*----------------   Активная   зона    --------------------------------*/
    //     Длины участков, м
    static  const   double  len_az[96] = {
            0.05,   0.025,  0.155,  0.,             0.12,   0.,
            0.12,   0.,             0.12,   0.,             0.12,   0.,
            0.12,   0.,             0.12,   0.,             0.12,   0.,
            0.12,   0.,             0.12,   0.,             0.12,   0.,
            0.12,   0.,             0.12,   0.,             0.12,   0.,
            0.12,   0.,             0.12,   0.,             0.12,   0.,
            0.12,   0.,             0.12,   0.,             0.12,   0.,
            0.12,   0.,             0.12,   0.,             0.12,   0.,
            0.12,   0.,             0.12,   0.,             0.12,   0.,
            0.12,   0.,             0.12,   0.,             0.005,  0.01,
            0.015,  0.015,  0.01,   0.005,  0.,             0.18,
            0.18,   0.,             0.18,   0.18,   0.,             0.18,
            0.18,   0.,             0.18,   0.18,   0.,             0.18,
            0.18,   0.,             0.18,   0.18,   0.,             0.18,
            0.18,   0.,             0.18,   0.18,   0.,             0.18,
            0.18,   0.,             0.1,    0.055,  0.025,  0.05
    };

    //   Коэф. местного сопротивления
    static  const   double  csi_az[96] = {
            0.45,   0.,             0.,             1.,             0.,             0.45,
            0.,             0.45,   0.,             1.,             0.,             0.45,
            0.,             0.45,   0.,             1.,             0.,             0.45,
            0.,             0.45,   0.,             1.,             0.,             0.45,
            0.,             0.45,   0.,             1.,             0.,             0.45,
            0.,             0.45,   0.,             1.,             0.,             0.45,
            0.,             0.45,   0.,             1.,             0.,             0.45,
            0.,             0.45,   0.,             1.,             0.,             0.45,
            0.,             0.45,   0.,             1.,             0.,             0.45,
            0.,             0.45,   0.,             1.,             0.,             0.,
            0.,             0.,     0.,             0.,             0.5,    0.,
            0.,             0.5,    0.,             0.,             0.5,    0.,
            0.,             0.5,    0.,             0.,             0.5,    0.,
            0.,             0.5,    0.,             0.,             0.5,    0.,
            0.,             0.5,    0.,             0.,             0.5,    0.,
            0.,             0.5,    0.,             0.,             0.,             0.
    };

    double  i0;        // энтальпия воды на входе в канал, ккал/кг
    double  g0;        // удельный объем недогретой воды на входе в канал, м**3/кг
    double  dPm, dPt, dPn, dPy = 0.;
    double  z = 0.;
    double  p, nu1, x;
    double  psi_m, psi_t, R, fi, Re, om1, om2, om, mu1;
    double  lambda, gamma;
    double  temp, temp1;
    double  zp, e;
    double  sigma, w, q;
    double  i, i_boil, i_liq, vy, vy_old = 0.;
    double  Wt[71], Qt[71], Zt[71];
    double  length = 0.0;
    double  delta;
    int     m, j;
    int     k = 118;

    for (j = 0; j < 71; j++) F[j] = 1.0; // перенесено из gidra_Start

    Lpvk = Lpvk + H_AZ;
    i0 = li1(12, t_l, i_t, t0);
    g0 = li1(12, i_t, v_t, i0);
    G = G / g0;

    N *= 1000.;
    p = p0 * 10000.;
    delta = 29.0 - Lpvk;
    len_pvk[3] -= delta;
    i = i0 + 860. * N / G;

    for (j = 0; j < 23; j++)  length += len_pvk[j];
    length += H_AZ;

    for (j = 0; j < 23; j++, k--)
    {
        spl(p, &Ts[k], &i1, &v1, &gamma1, &i2, &v2, &gamma2, &r);
        mu1 = li1(12, p_t, mu1_t, p * 0.0001);
        v3 = li1(12, i_t, v_t, i);
        nu1 = grav * mu1 * v1;
        x = (i - i1) / r;
        if (x <= 0.00001)
        {
            one_phase(i, G, d_pvk[j], s_pvk[j], &psi_m, &psi_t,
                &R, &fi, &Re, &gamma, &om1, &om);
            fi = 0;
            om2 = om1;
        }
        else
        {
            two_phase(p, x, nu1, G, d_pvk[j], s_pvk[j],
                &psi_m, &psi_t, &R, &fi, &Re, &gamma, &om1, &om2, &om);
            if (fi > 0.999)
                fi = 0.999;
        }
        lambda = 0.1 * sqrt(sqrt(1.46 * sh_pvk[j] / d_pvk[j] + 100. / Re));
        temp = G * G * v1 / (3600. * 3600. * s_pvk[j] * s_pvk[j] * 2. * grav);
        dPm = csi_pvk[j] * psi_m * temp * R;
        dPt = lambda * len_pvk[j] / d_pvk[j] * psi_t * temp * R;
        dPn = h_pvk[j] * gamma;
        p += dPm + dPt + dPn;
        Ze[k] = length - len_pvk[j] * 0.5;
        Gamma[k] = gamma;
        S[k] = s_pvk[j];
        P[k] = p;
        Gt[k] = fi * 100.0;
        OM1[k] = .1 / om1;
        OM2[k] = .1 / om2;
        OM[k] = .1 / om;
        SR0[k] = S[k] / v3;
        SR01[k] = S[k] / v1;
        QONT[k] = (v1 * N) / (4.1868 * H_AZ * r * S[k] * (1 - fi) * (om1));
        length -= len_pvk[j];
    }     // for
    e = i;
    Wt[0] = Zt[0] = 0.;
    sigma = F[0] / 2.0 + F[70] / 2.0;
    for (j = 1; j < 70; j++)
        sigma += F[j];
    for (j = 0; j < 71; j++)
        Qt[j] = 860. * N * K_cell * (1. - eta_gr) * F[j] * 70. / (F_w * sigma);
    for (j = 1; j < 71; j++) {
        Zt[j] = Zt[j - 1] + H_AZ / 70.;
        Wt[j] = Wt[j - 1] + 860. * N * (F[j - 1] + F[j]) / (2. * sigma);
    }
    m = 0;
    temp1 = G * G / (3600. * 3600. * S_az * S_az);
    for (j = 0; j < 96; j++, k--)
    {
        spl(p, &Ts[k], &i1, &v1, &gamma1, &i2, &v2, &gamma2, &r);
        q = li1(71, Zt, Qt, z);
        mu1 = li1(12, p_t, mu1_t, p * 0.0001);
        v3 = li1(12, i_t, v_t, i);
        nu1 = grav * mu1 * v1;
        x = (i - i1) / r;
        i_boil = i1 - (55. * q * S_az / G) * pow((v1 * d_tk * sqrt(q * G *
            v1 * v2 / (S_az * r)) / (v2 * nu1 * 3600.)), 0.3);
        if (i <= i_boil)
        {
            one_phase(i, G, d_tk, S_az, &psi_m, &psi_t, &R, &fi,
                &Re, &gamma, &om1, &om);
            vy = 0.;
        }
        else
        {
            i_liq = i1 - ((i - i_boil) + (i1 - i_boil)) *
                exp(-2. * (i - i_boil) / (i1 - i_boil));
            x = (i - i_liq) / (i2 - i_liq);
            if (x <= 0.00001)
            {
                one_phase(i, G, d_tk, S_az, &psi_m, &psi_t, &R, &fi, &Re,
                    &gamma, &om1, &om);
                vy = 0.;
            }
            else
            {
                two_phase(p, x, nu1, G, d_tk, S_az, &psi_m, &psi_t, &R, &fi,
                    &Re, &gamma, &om1, &om2, &om);
                if (fi > 0.999)
                    fi = 0.999;
                vy = x * x * v2 / fi + (1. - x) * (1. - x) *
                    v1 / (1. - fi);
            }
        }                 // i > i_boil
        lambda = 0.1 * sqrt(sqrt(1.46 * sh / d_tk + 100. / (Re * hi)));
        if (i > i_boil)
            m++;
        z += len_az[j];
        if (j > 0)
            dPy = temp1 * (vy_old - vy) / grav;
        vy_old = vy;
        temp = temp1 * v1 / (2. * grav);
        dPm = csi_az[j] * psi_m * temp * R;
        dPt = lambda * len_az[j] / d_tk * psi_t * temp * R;
        dPn = len_az[j] * gamma;
        p += dPm + dPt + dPn + dPy;
        w = li1(71, Zt, Wt, z);
        i = e - w / G;
        Ze[k] = length - len_az[j] * 0.5;
        Gamma[k] = gamma;
        S[k] = S_az;
        P[k] = p;
        Gt[k] = fi * 100.0;
        OM[k] = .1 / om;
        OM1[k] = .1 / om1;
        OM2[k] = .1 / om2;
        SR0[k] = S[k] / v3;
        SR01[k] = S[k] / v1;
        QONT[k] = (v1 * N) / (4.1868 * H_AZ * r * S[k] * (1 - fi) * (om1));
        length -= len_az[j];
    }
    zp = 0;
    for (j = 0; j < m; j++)
        zp += len_az[j];
    len_pvk[3] += delta;
    strt_boil = H_AZ - zp;
}

double  Padonel(double G, double N, double p0, double t0, double Lpvk)
/*-------------------------------------------------------------------------*/
/*                              Входы                                      */
/*    G  - расход теплоносителя, м3/час                                     */
/*    N  - мощность канала,   МВт                                           */
/*    p0 - давление в барабан-сепараторе, кг/см2                            */
/*    t0 - температура воды на входе в канал, С                             */
/*    Lpvk - длина пароводяной коммуникации, м                              */
/*                              Выходы                                      */
/*   Padonel - перепад давления от входа в АЗ до БС, кг/см2                 */
/*-------------------------------------------------------------------------*/

{

    gidra(G, N, p0, t0, Lpvk);

    return P[0] - P[118];
} //  Padonel



