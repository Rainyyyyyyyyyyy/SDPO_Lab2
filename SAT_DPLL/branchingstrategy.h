#ifndef BRANCHINGSTRATEGY_H
#define BRANCHINGSTRATEGY_H

class BoolEquation;

class BranchingStrategy {
public:

    virtual ~BranchingStrategy() = default;
    virtual int ChooseColumn(BoolEquation &equation) const = 0;

};


// предложенная из пользовательского проекта стратегия выбора
class MostContraintBranchingStrategy : public BranchingStrategy {
public:
    int ChooseColumn(BoolEquation &equation) const override;
};


// первая слева незафиксированная переменная
// если нет, то return -1
class FirstFreeBranchingStrategy : public BranchingStrategy {
public:
    int ChooseColumn(BoolEquation &equation) const override;
};

// первая справа незафиксированная переменная
// если нет, то return -1
class LastFreeBranchingStrategy : public BranchingStrategy {
public:
    int ChooseColumn(BoolEquation &equation) const override;
};

#endif // BRANCHINGSTRATEGY_H
