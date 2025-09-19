#include <iostream>

using namespace std;

template <typename T>
class myunique_ptr {
    public:
        myunique_ptr() = default;

        myunique_ptr(T *t) {
            m_ptr = t;
            cout << "ctor(t)\n";
        }

        ~myunique_ptr() {
            cout << "dtor()\n";
            delete m_ptr;
        }

        myunique_ptr(const myunique_ptr& ) = delete;

        myunique_ptr(myunique_ptr&& other) {
            m_ptr = other.m_ptr;
            other.m_ptr = nullptr;
        }

        myunique_ptr& operator=(myunique_ptr& copy) = delete;

        myunique_ptr& operator=(myunique_ptr&& other) {
            if (this != &other) {
                delete m_ptr;            
                m_ptr = other.m_ptr;
                other.m_ptr = nullptr;
            }
            return *this;
        }

        T* get() {
            return m_ptr;
        }

        T* operator->() const {
            if (nullptr == m_ptr)
                throw std::runtime_error("Dereferencing nullptr");
            return m_ptr;
        }

        T& operator*() const {
            if (nullptr == m_ptr) {
                throw std::runtime_error("Dereferencing nullptr");
            }
            return *m_ptr;
        }

        T* release() {
            T* temp = m_ptr;
            m_ptr = nullptr;
            return temp;
        }

        friend void swap(myunique_ptr<T>& a, myunique_ptr<T>& b) {
            T* temp = a.m_ptr;
            a.m_ptr = b.m_ptr;
            b.m_ptr = temp;
        }

    private:
        T *m_ptr = nullptr;
};

