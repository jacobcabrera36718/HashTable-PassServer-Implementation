
template <typename K, typename V>
HashTable<K, V>::HashTable(size_t size)
{
  currentSize = 0;
  theLists.resize( prime_below(size) );
}

template <typename K, typename V>
HashTable<K, V>::~HashTable()
{
  clear();
}

template <typename K, typename V>
bool HashTable<K, V>::contains(const K & k)const
{
  for(auto itr = theLists[myhash(k)].begin(); itr != theLists[myhash(K)].end(); ++itr)
    {
      if(itr->first == k)
        return true;
    }
    return false;
}

template <typename K, typename V>
bool HashTable<K, V>::match(const std::pair<K, V> & kv) const
{
  for(auto itr = theLists[myhash(kv.first)].begin(); itr != theLists[myhash(kv.first)].end(); ++itr)
    {
      if(itr->first == kv.first)
        return true;
    }
    return false;
}

template <typename K, typename V>
bool HashTable<K, V>::insert(const std::pair<K, V> & kv)
{
  if(match(kv))
    return false;
  if(contains(kv.first))
    return false;
  theLists[myhash(kv.first)].push_back(kv);
  ++currentSize;
  if(currentSize > theLists.size())
    rehash();
  return true;
}

template <typename K, typename V>
bool HashTable<K, V>::insert(std::pair<K, V> && kv)
{
  if(match(kv))
    return false;
  if(contains(kv.first))
    return false;
  theLists[myhash(kv.first)].push_back(std::move(kv));
  ++currentSize;
  if(currentSize > theLists.size())
    rehash();
  return true;
}

template <typename K, typename V>
bool HashTable<K, V>::remove(const K & k)
{
  bool flag = false;
  if(!contains(k))
  {
    return flag3;
  }
  auto & whichList = theLists[myhash(k)];
for(auto itr = whichList.begin(); itr != whichList.end(); ++itr)
    {
      if(itr->first == k )
        {
          itr = whichList.earse(itr);
          --currentSize;
          flag = true;
        }
    }
    return flag;
}

template <typename K, typename V>
void HashTable<K, V>::clear()
{
  makeEmpty();
}

template <typename K, typename V>
bool HashTable<K,V>::load( const char* filename )  {
	std::pair<K,V> kvpair;
	std::ifstream fin;
	fin.open( filename, std::ifstream::in );

	if( !fin )
		return false;

	char wspace;
	while( fin )  {
	// keep reading in kv pairs until reaching the end of file
		std::getline( fin, kvpair.first, ' ' );

		while( isspace( fin.peek() ) )
			fin.get( wspace );
		std::getline( fin, kvpair.second, '\n' );

		while( isspace( fin.peek() ) )
			fin.get( wspace );

		insert( kvpair );
	}

	fin.close();
	return true;
}

template <typename K, typename V>
void HashTable<K, V>::dump() const
{
  for(int i = 0; i < theLists.size(); ++i)
    {
      std::cout << "v[" << i << "]: ";
      for(auto itr = theLists[i].begin(); itr != theLists[i].end(); ++itr)
        {
          std::cout << itr->first << " " << itr->second << " ";
        }
      std::cout << '\n';
    }
}

template <typename K, typename V>
size_t HashTable<K, V>::size() const
{
  return currentSize;
}

template <typename K, typename V>
bool HashTable<K, V>::write_to_file(const char *filename) const
{
  std::ofstream fout;
  fout.open(filename);
  if(!fout)
  {
    return false;
  }
  for(int i = 0; i < theLists.size(); ++i)
  {
    for(auto itr = theLists[i].begin(); itr != theLists[i].end(); ++itr)
    {
      fout << itr->first << " " << itr->second << '\n';
    }
  }
  fout.close();
  return true;
}

template <typename K, typename V>
void HashTable<K, V>::makeEmpty()
{
  for(auto & thisList : theLists)
    {
      thisList.clear();
    }
    currentSize = 0;
}

template <typename K, typename V>
void HashTable<K, V>::rehash()
{
  vector<list<pair<K, V>>> oldLists = theLists;
  theLists.resize(prime_below(2*theLists.size()));
  for(auto & thisList : theLists)
    {
      thisList.clear();
    }
    currentSize = 0;
    for(auto & thisList : oldLists)
    {
      for(auto & x : thisList)
      {
        insert(std::move(x));
      }
    }
}

template <typename K, typename V>
size_t HashTable<K, V>::myhash(const K & k) const
{
  static hash<K> hf;
  return hf(k) % theLists.size();
}

template <typename K, typename V>
unsigned long HashTable<K, V>::prime_below(unsigned long n)
{
  if (n > max_prime)
    {
      std::cerr << "** input too large for prime_below()\n";
      return 0;
    }
  if (n == max_prime)
    {
      return max_prime;
    }
  if (n <= 1)
    {
		std::cerr << "** input too small \n";
      return 0;
    }

  // now: 2 <= n < max_prime
  std::vector <unsigned long> v (n+1);
  setPrimes(v);
  while (n > 2)
    {
      if (v[n] == 1)
	return n;
      --n;
    }

  return 2;
}

template <typename K, typename V>
void HashTable<K, V>::setPrimes(vector<unsigned long> & vprimes)
{
  int i = 0;
  int j = 0;

  vprimes[0] = 0;
  vprimes[1] = 0;
  int n = vprimes.capacity();

  for (i = 2; i < n; ++i)
    vprimes[i] = 1;

  for( i = 2; i*i < n; ++i)
    {
      if (vprimes[i] == 1)
        for(j = i + i ; j < n; j += i)
          vprimes[j] = 0;
    }
}

