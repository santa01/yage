class Test {
public:
    Test(bool ok) : m_ok(ok) {}

    Test() : Test(true) {}

    bool isOk() const { return m_ok; }

private:
    bool m_ok;
};

int main() {
    Test t;

    return t.isOk() ? 0 : 1;
}
