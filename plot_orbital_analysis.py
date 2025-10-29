import pandas as pd
import matplotlib.pyplot as plt
import numpy as np


MU = 3.986004415e14
J2 = 1.633e-3
R_earth = 6378137.0

def calculate_shift_raan(p, i):
    epsilon = J2 * MU * (R_earth ** 2)
    d_raan = (-2 * np.pi * epsilon / (MU * (p ** 2))) * np.cos(i)
    return d_raan

def calculate_perigee_shift(p, i):
    sin2_i = np.sin(i) ** 2
    epsilon = J2 * MU * (R_earth ** 2)
    d_w = (2 * np.pi * epsilon / (MU * (p ** 2))) * (2 - 2.5 * sin2_i)
    return d_w

def load_orbital_data(filename):
    df = pd.read_csv(filename, sep=';')
    df['t_days'] = df['t'] / 86400.0
    return df

def calculate_orbital_parameters(data):
    a0 = data['a'].iloc[0]
    e0 = data['e'].iloc[0]
    i0 = data['i'].iloc[0]
    
    p0 = a0 * (1.0 - e0 * e0)
    T0 = 2.0 * np.pi * np.sqrt((a0 ** 3) / MU)
    N_orbits = data['t'].to_numpy() / T0
    
    return p0, i0, N_orbits

def plot_semi_major_axis(data: pd.DataFrame, orbit_name, prefix):

    plt.figure(figsize=(10, 6))
    plt.plot(data['t_days'], data['a'] / 1000.0, color='#1f77b4', linewidth=2)
    plt.xlabel('Время (дни)')
    plt.ylabel('Большая полуось (км)')
    plt.title(f'{orbit_name}: Большая полуось vs время')
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    fname = f'{prefix}_a_vs_time.png'
    plt.savefig(fname, dpi=300, bbox_inches='tight')
    print(f'Сохранено: {fname}')
    plt.close()

def plot_raan(data, orbit_name, prefix):

    p0, i0, N_orbits = calculate_orbital_parameters(data)
    
    delta_raan = calculate_shift_raan(p0, i0)

    raan0 = data['raan'].iloc[0]
    raan_trend_deg = np.degrees(raan0 + N_orbits * delta_raan)
    t_days = data['t_days'].to_numpy()
    rann_deg = np.degrees(data['raan'].to_numpy())
    a_w, b_w = np.polyfit(t_days, rann_deg, 1)
    rann_linear_trend = a_w * t_days + b_w

    plt.figure(figsize=(10, 6))
    plt.plot(t_days, np.degrees(data['raan']), color='#1f77b4', linewidth=2)
    plt.plot(t_days, raan_trend_deg, color='#d62728', linestyle='--', linewidth=2, label='Тренд теоретический')
    plt.plot(t_days, rann_linear_trend, color='#2ca02c', linestyle='-.', linewidth=2, label='Тренд рассчитаный')
    plt.xlabel('Время (дни)')
    plt.ylabel('Долгота восходящего узла (град)')
    plt.title(f'{orbit_name}: Долгота восходящего узла vs время')
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    fname = f'{prefix}_raan_vs_time.png'
    plt.savefig(fname, dpi=300, bbox_inches='tight')
    print(f'Сохранено: {fname}')
    plt.close()

def plot_inclination(data, orbit_name, prefix):

    plt.figure(figsize=(10, 6))
    plt.plot(data['t_days'], np.degrees(data['i']), color='#1f77b4', linewidth=2)
    plt.xlabel('Время (дни)')
    plt.ylabel('Наклонение (град)')
    plt.title(f'{orbit_name}: Наклонение vs время')
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    fname = f'{prefix}_i_vs_time.png'
    plt.savefig(fname, dpi=300, bbox_inches='tight')
    print(f'Сохранено: {fname}')
    plt.close()

def plot_argument_of_perigee(data, orbit_name, prefix):

    p0, i0, N_orbits = calculate_orbital_parameters(data)
    delta_perigee_per_orbit = calculate_perigee_shift(p0, i0)
    w0 = data['w'].iloc[0]
    w_trend_deg = np.degrees(w0 + N_orbits * delta_perigee_per_orbit)
    t_days = data['t_days'].to_numpy()

    w_deg = np.degrees(data['w'].to_numpy())
    a_w, b_w = np.polyfit(t_days, w_deg, 1)
    w_linear_trend = a_w * t_days + b_w

    plt.figure(figsize=(10, 6))
    w_wrapped_deg = np.degrees(data['w'].to_numpy())
    plt.plot(t_days, w_wrapped_deg, color='#1f77b4', linewidth=2, label='Аргумент перицентра')
    plt.plot(t_days, w_trend_deg, color='#d62728', linestyle='--', linewidth=2, label='Тренд теоретический')
    plt.plot(t_days, w_linear_trend, color='#2ca02c', linestyle='-.', linewidth=2, label='Тренд рассчитаный')
    
    plt.xlabel('Время (дни)')
    plt.ylabel('Аргумент перицентра (град)')
    plt.title(f'{orbit_name}: Аргумент перицентра vs время')
    plt.grid(True, alpha=0.3)
    plt.legend()
    plt.tight_layout()
    fname = f'{prefix}_w_vs_time.png'
    plt.savefig(fname, dpi=300, bbox_inches='tight')
    print(f'Сохранено: {fname}')
    plt.close()

def plot_eccentricity(data, orbit_name, prefix):

    plt.figure(figsize=(10, 6))
    plt.plot(data['t_days'], data['e'], color='#1f77b4', linewidth=2)
    plt.xlabel('Время (дни)')
    plt.ylabel('Эксцентриситет')
    plt.title(f'{orbit_name}: Эксцентриситет vs время')
    plt.grid(True, alpha=0.3)
    plt.tight_layout()
    fname = f'{prefix}_e_vs_time.png'
    plt.savefig(fname, dpi=300, bbox_inches='tight')
    print(f'Сохранено: {fname}')
    plt.close()

def create_plots_for_orbit(data, orbit_name, prefix):

    plt.style.use('seaborn-v0_8')

    plot_semi_major_axis(data, orbit_name, prefix)
    plot_raan(data, orbit_name, prefix)
    plot_inclination(data, orbit_name, prefix)
    plot_argument_of_perigee(data, orbit_name, prefix)
    plot_eccentricity(data, orbit_name, prefix)

def create_all_orbit_plots():

    try:
        orb1_data = load_orbital_data('res_orb1.csv')
        orb2_data = load_orbital_data('res_orb2.csv')
    except FileNotFoundError as e:
        print(f"Ошибка: Не удалось найти файлы данных. {e}")
        return

    create_plots_for_orbit(orb1_data, 'Орбита 1', 'orbit1')
    create_plots_for_orbit(orb2_data, 'Орбита 2', 'orbit2')

if __name__ == "__main__":
    create_all_orbit_plots()