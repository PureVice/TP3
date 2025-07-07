import subprocess
import time
import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns
import os
import glob
import sys
import numpy as np

def compile_code():
    """Compila o simulador C++ e o gerador de dados C."""
    print("\n[🛠] Iniciando compilação do código-fonte para TPEXTRA...")
    start_time = time.time()
    
    try:
        # Compila o gerador de dados
        print("  - Compilando o gerador de dados (genwkl3)...")
        gen_path = "genwkl3.c"
        if not os.path.exists(gen_path):
            raise FileNotFoundError("Arquivo 'genwkl3.c' não encontrado.")
        
        result = subprocess.run(
            ["gcc", "-O2", "-o", "genwkl3", gen_path, "-lm"],
            check=True, capture_output=True, text=True
        )
        if result.stderr:
            print(f"    ⚠️ Avisos na compilação do genwkl3:\n{result.stderr}")

        # Compila o simulador C++
        print("  - Compilando o simulador C++ (TPEXTRA)...")
        source_files = glob.glob("src/*.cc")
        if not source_files:
            raise FileNotFoundError("Nenhum arquivo .cc encontrado em src/")
        
        command = [
            "g++", "-std=c++11", "-O2", "-Wall",
            "-I./include",
            "-o", "simulador"
        ] + source_files
        result = subprocess.run(command, check=True, capture_output=True, text=True)
        if result.stderr:
            print(f"    ⚠️ Avisos na compilação do simulador:\n{result.stderr}")

        compile_time = time.time() - start_time
        print(f"==> ✅ Compilação bem-sucedida em {compile_time:.2f} segundos.")
        return True

    except (FileNotFoundError, subprocess.CalledProcessError) as e:
        print("❌ ERRO durante a compilação:")
        if hasattr(e, 'stderr'): print(e.stderr)
        else: print(e)
        sys.exit(1)


def generate_input_file(nodes, packets, clients, seed):
    """Executa o gerador de dados para criar 'tp3.in' e ordena o arquivo."""
    try:
        # Executa o gerador que cria 'tp3.in'
        subprocess.run(
            ["./genwkl3", "-s", str(seed), "-n", str(nodes), "-p", str(packets), "-c", str(clients), "-t", "10"],
            check=True, capture_output=True, text=True
        )
        
        # Ordena o arquivo de entrada, que é crucial para o processamento sequencial de tempo
        if os.path.exists("tp3.in"):
            with open("tp3.in", "r") as f:
                content = f.readlines()
            
            content.sort() # Ordena as linhas (eventos e consultas) por timestamp
            
            with open("tp3.in", "w") as f:
                f.writelines(content)
        else:
            raise FileNotFoundError("tp3.in não foi gerado.")
        return True
    except (subprocess.CalledProcessError, FileNotFoundError) as e:
        print(f"❌ ERRO ao gerar arquivo de entrada: {e}")
        if hasattr(e, 'stderr'): print(e.stderr)
        return False


def run_simulation(input_file):
    """Executa uma simulação e retorna as métricas de desempenho."""
    if not (os.path.exists("./simulador") and os.path.exists(input_file)):
        return None
    try:
        start_time = time.time()
        result = subprocess.run(
            ["./simulador", input_file],
            check=True, capture_output=True, text=True, encoding='utf-8'
        )
        exec_time = time.time() - start_time
        output_lines = result.stdout.splitlines()
        # Contabiliza todas as consultas, incluindo as novas
        total_consultas = len([
            line for line in output_lines 
            if "CL " in line or "PC " in line or "MA " in line or "RC " in line
        ])
        return {"exec_time": exec_time, "total_consultas": total_consultas}
    except subprocess.CalledProcessError as e:
        print(f"❌ Erro na simulação: {e.stderr.strip()}")
        return None


def plot_2d_analysis(df, x_var, y_var, title, filename, log_scale=False, x_min=None, y_min=None):
    """Gera um gráfico 2D focado para documentação."""
    if df.empty:
        print(f"⚠️ AVISO: Sem dados para plotar '{filename}'")
        return
        
    plt.style.use('seaborn-v0_8-whitegrid')
    plt.figure(figsize=(10, 6))
    
    if log_scale:
        plt.xscale('log')
        plt.yscale('log')
    
    sns.regplot(
        data=df, x=x_var, y=y_var,
        scatter=True, ci=95,
        scatter_kws={'alpha': 0.5, 's': 50, 'color': 'darkgreen'}, # Cor alterada para diferenciar
        line_kws={'color': 'darkred', 'linewidth': 2}
    )
    
    plt.title(title, fontsize=16, fontweight='bold', pad=15)
    plt.xlabel(x_var.replace('_', ' ').capitalize(), fontsize=12)
    plt.ylabel(y_var.replace('_', ' ').capitalize(), fontsize=12)
    
    if x_min is not None:
        plt.xlim(left=x_min)
    if y_min is not None:
        plt.ylim(bottom=y_min)
    
    plt.grid(True, which="both", ls="--", c='gray', alpha=0.5)
    plt.tight_layout()
    plt.savefig(filename, dpi=150, bbox_inches='tight')
    print(f"==> 📄 Gráfico de análise salvo em '{filename}'")
    plt.close()


def main():
    """Orquestra a análise experimental completa."""
    print("\n" + "="*70)
    print("      ANÁLISE DE DESEMPENHO E ESCALABILIDADE DO SIMULADOR (TPEXTRA)")
    print("="*70 + "\n")

    if not compile_code():
        sys.exit(1)

    # --- Configurações do Experimento ---
    repetitions = 5
    rng = np.random.default_rng(seed=2024)
    results_data = []

    # --- Experimento 1: Variação de Pacotes e Clientes ---
    print("\n[ 1 ] Análise de Escalabilidade por Carga de Trabalho")
    print("-" * 50)
    packets_range = [100, 500, 1000, 5000, 10000, 25000]
    clients_range = [10, 50, 100, 250, 500, 1000]
    
    for packets in packets_range:
        for clients in clients_range:
            nodes = max(50, int(packets/100))
            print(f"  Testando: {packets} pacotes, {clients} clientes...")
            
            for _ in range(repetitions):
                seed = int(rng.integers(1, 2**30))
                if generate_input_file(nodes=nodes, packets=packets, clients=clients, seed=seed):
                    metrics = run_simulation("tp3.in")
                    if metrics:
                        results_data.append({
                            "packets": packets,
                            "clients": clients,
                            "nodes": nodes,
                            "exec_time": metrics["exec_time"],
                        })

    # --- Experimento 2: Variação de Densidade ---
    print("\n[ 2 ] Análise de Escalabilidade por Densidade da Rede")
    print("-" * 50)
    densities = [0.01, 0.1, 0.5, 1.0, 2.0, 5.0, 10.0]
    base_nodes = 100
    
    for density in densities:
        clients = max(1, int(base_nodes * density))
        packets = clients * 100
        print(f"  Testando: Densidade {density:.2f} ({clients} clientes)...")
        
        for _ in range(repetitions):
            seed = int(rng.integers(1, 2**30))
            if generate_input_file(nodes=base_nodes, packets=packets, clients=clients, seed=seed):
                metrics = run_simulation("tp3.in")
                if metrics:
                    results_data.append({
                        "packets": packets,
                        "clients": clients,
                        "nodes": base_nodes,
                        "densidade": density,
                        "exec_time": metrics["exec_time"],
                    })

    if not results_data:
        print("\n⚠️ Nenhum resultado de simulação foi coletado. Encerrando.")
        return

    # --- Processamento e Geração de Gráficos ---
    full_df = pd.DataFrame(results_data)
    results_file = "analise_resultados_tpextra.csv"
    full_df.to_csv(results_file, index=False)
    
    print("\n" + "="*70)
    print(f"✅ Análise (TPEXTRA) concluída. Resultados salvos em '{results_file}'")
    print("="*70 + "\n")

    print("[ 3 ] Gerando gráficos para documentação...")
    
    plot_2d_analysis(
        full_df, 
        x_var="packets", 
        y_var="exec_time", 
        title="Desempenho (TPEXTRA) vs. Volume de Pacotes", 
        filename="analise_tpextra_tempo_vs_pacotes.png",
        log_scale=True, x_min=100, y_min=0.001
    )
    
    density_df = full_df[full_df['densidade'].notna()].copy()
    if not density_df.empty:
        plot_2d_analysis(
            density_df,
            x_var="densidade",
            y_var="exec_time",
            title="Desempenho (TPEXTRA) vs. Densidade de Clientes",
            filename="analise_tpextra_tempo_vs_densidade.png",
            log_scale=True, x_min=0.01, y_min=0.001
        )

    # --- Limpeza ---
    print("\n[ 4 ] Limpando arquivos temporários...")
    files_to_clean = ["genwkl3", "simulador", "tp3.in"]
    for f in files_to_clean:
        if os.path.exists(f): 
            os.remove(f)
    print("==> Limpeza concluída.")

if __name__ == "__main__":
    main()