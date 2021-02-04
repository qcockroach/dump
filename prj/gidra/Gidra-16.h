#ifndef GIDRA_H
#define GIDRA_H


const double grav = 9.81;
const double H_AZ = 7.0;                 /* ������  ��,� */
const double d_tk = 0.8562e-2;       /* ����. ������� �� � ���,�             */
const double eta_gr = 0.054;           /* ���� �����, ������� �� �������       */
const double F_w = 5.35;        /* ����������� ����������� ���,�**2     */
const double hi = 0.561;           /* ��������� ��� � �����                */
const double K_cell = 1.0;                 /* ����. ��������������� �������������� */
const double P_kr = 225.65e+4;   /* ����������� ��������                 */
const double S_az = 0.2273e-2;       /* ��������� ������� ��,�**2            */
const double sh = 1.0e-5;          /* ������������� ������ � ��            */

const double alf = 0.07; /* ���� �������� �� ��������� */

#endif


#ifdef __cplusplus
extern "C" {
#endif  /* __cplusplus */

	void gidra_Start(void);

	double Get_F(int i);
	double Get_Gamma(int i);
	double Get_Gt(int i);
	double Get_Ze(int i);
	double Get_Ts(int i);
	double Get_S(int i);
	double Get_OM1(int i);
	double Get_OM2(int i);
	double Get_P(int i);
	double Get_SR0(int i);
	double Get_SR01(int i);
	double Get_QONT(int i);
	double Get_OM(int i);
	double Get_strt_boil(void);
	double Get_Na(int i);
	double Get_Naone(int i);
	double Get_q1one(int i);
	double Get_q1(int i);
	double Get_q2(int i);
	double Get_CDe(int i);
	double Get_Param_A(void);
	void Set_Param_A(double);
	double Get_m_az(void);
	void Set_m_az(double);
	double Get_m_pvk(void);
	void Set_m_pvk(double);
	double Get_dq(int i);
	double Get_dn(int i);
	double Get_POMZL(int);
	double Get_PSS(int);
	double Get_PD(int);
	double Get_GtL(int);

	double li1(int, const double[], const double[], double);

	void gidra(double, double, double, double, double);

	double Padonel(double, double, double, double, double);

#ifdef __cplusplus
}                       /* End of extern "C" { */
#endif  /* __cplusplus */


