#include "branchingstrategy.h"
#include "boolequation.h"

#include <algorithm>
#include <vector>

// предложенная из пользовательского проекта стратегия выбора
int MostContraintBranchingStrategy::ChooseColumn(BoolEquation &equation) const
{
	std::vector<int> indexes;
	std::vector<int> values;
	bool rezInit = false;

    for (int i = 0; i < equation.mask.getSize(); i++) {
        if (equation.mask[i] == 0) {
			indexes.push_back(i);
		}
	}

	if (indexes.empty()) {
		return -1;
	}

	for (int i = 0; i < equation.cnfSize; i++) {
		BoolInterval *interval = equation.cnf[i];

		if (interval != nullptr) {
			if (!rezInit) {
				for (int k = 0; k < static_cast<int>(indexes.size()); k++) {
					if (interval->getValue(indexes.at(k)) == '-') {
						values.push_back(1);
					} else {
						values.push_back(0);
					}
				}

				rezInit = true;
			} else {
				for (int k = 0; k < static_cast<int>(indexes.size()); k++) {
					if (interval->getValue(indexes.at(k)) == '-') {
						values.at(k)++;
					}
				}
			}
		}
	}

	if (values.empty()) {
		return indexes.front();
	}

    int minElementIndex = std::min_element(values.begin(), values.end()) - values.begin();

	return indexes.at(minElementIndex);
}

// первая слева незафиксированная переменная
// если нет, то return -1
int FirstFreeBranchingStrategy::ChooseColumn(BoolEquation &equation) const
{
	for (int i = 0; i < equation.mask.getSize(); i++) {
		if (equation.mask[i] == 0) {
			return i;
		}
	}

	return -1;
}

// первая справа незафиксированная переменная
// если нет, то return -1
int LastFreeBranchingStrategy::ChooseColumn(BoolEquation &equation) const
{
	for (int i = equation.mask.getSize() - 1; i >= 0; i--) {
		if (equation.mask[i] == 0) {
			return i;
		}
	}

	return -1;
}
