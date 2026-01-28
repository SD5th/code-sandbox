from itertools import combinations_with_replacement

elements = ["Чёрная", "Кровавая", "Бурая", "Савьюр", "Белая Плеть", "-"]

all_combinations = list(combinations_with_replacement(elements, 4))

sorted_combinations = []
for combo in all_combinations:
    sorted_combo = sorted(combo, key=lambda x: elements.index(x))
    sorted_combinations.append(sorted_combo)

sorted_combinations = [tuple(combo) for combo in sorted_combinations]

unique_combinations = list(set(sorted_combinations))

unique_combinations.sort(key=lambda x: [elements.index(item) for item in x])

print(f"Всего уникальных комбинаций: {len(unique_combinations)}")

col1 = [combo[0] for combo in unique_combinations]
col2 = [combo[1] for combo in unique_combinations]
col3 = [combo[2] for combo in unique_combinations]
col4 = [combo[3] for combo in unique_combinations]

print("\n" + "="*50)
print("СТОЛБЕЦ 1:")
print("="*50)
for i, item in enumerate(col1, 1):
    print(f"{item}")


print("\n" + "="*50)
print("СТОЛБЕЦ 2:")
print("="*50)
for i, item in enumerate(col2, 1):
    print(f"{item}")


print("\n" + "="*50)
print("СТОЛБЕЦ 3:")
print("="*50)
for i, item in enumerate(col3, 1):
    print(f"{item}")


print("\n" + "="*50)
print("СТОЛБЕЦ 4:")
print("="*50)
for i, item in enumerate(col4, 1):
    print(f"{item}")


print("\n" + "="*50)
print("ПОЛНЫЕ СТРОКИ:")
print("="*50)
for i, combo in enumerate(unique_combinations, 1):
    print(f"{i:3}. {combo[0]:15} {combo[1]:15} {combo[2]:15} {combo[3]:15}")