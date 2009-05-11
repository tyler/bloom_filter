require File.dirname(__FILE__) + '/../ext/bloom_filter/bloom_filter'

describe BloomFilter do
  describe :new do
    it 'returns an instance of BloomFilter' do
      BloomFilter.new(1024, 3).should be_an_instance_of(BloomFilter)
    end
  end

  describe :set do
    it 'accepts a key of any type' do
      filter = BloomFilter.new(1024,3)
      lambda { filter.set(123) }.should_not raise_error
      lambda { filter.set('abc') }.should_not raise_error
      lambda { filter.set(:foo) }.should_not raise_error
      lambda { filter.set([1,2,3]) }.should_not raise_error
      lambda { filter.set(:a => 1) }.should_not raise_error
      lambda { filter.set(nil) }.should_not raise_error
    end

    it 'returns true' do
      filter = BloomFilter.new(1024,3)
      filter.set(123).should be_true
    end
  end

  describe :get do
    before :each do
      @filter = BloomFilter.new(1024,3)
      @filter.set(123)
      @filter.set('abc')
      @filter.set(:foo)
      @filter.set([1,2,3])
      @hash = { :a => 1 }
      @filter.set(@hash)
      @filter.set(nil)
    end

    it 'returns nil when the key does not exist' do
      @filter.get('not in the filter').should be_nil
    end

    it 'returns true when the key exists' do
      @filter.get(123).should be_true
      @filter.get('abc').should be_true
      @filter.get(:foo).should be_true
      @filter.get([1,2,3]).should be_true
      @filter.get(@hash).should be_true
      @filter.get(nil).should be_true
    end
  end

  describe :filter_size do
    it 'returns the size of the bitset used in the filter' do
      f = BloomFilter.new(1024,3)
      f.filter_size.should == 1024

      f = BloomFilter.new(10,3)
      f.filter_size.should == 10
    end
  end

  describe :hash_count do
    it 'returns the number of hash functions used in the filter' do
      f = BloomFilter.new(1024,3)
      f.hash_count.should == 3

      f = BloomFilter.new(1024,10)
      f.hash_count.should == 10
    end
  end
end
