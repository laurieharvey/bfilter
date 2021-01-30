namespace bloom
{
    template <typename Key>
    class bfilter
    {
    public:
        using key_type = Key;
        using value_type = Key;

        explicit bfilter();

        void insert(const value_type &value);

        bool contains(const key_type &key) const;

        bool empty() const;
    };
} // namespace bloom
