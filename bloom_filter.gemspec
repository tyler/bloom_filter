# -*- encoding: utf-8 -*-

Gem::Specification.new do |s|
  s.name = %q{bloom_filter}
  s.version = "0.1.1"

  s.required_rubygems_version = Gem::Requirement.new(">= 0") if s.respond_to? :required_rubygems_version=
  s.authors = ["Tyler McMullen"]
  s.date = %q{2009-05-11}
  s.description = %q{Fast Bloom filter in C and Ruby.}
  s.email = %q{tyler@scribd.com}
  s.extensions = ["ext/bloom_filter/extconf.rb"]
  s.files = ["README.textile", "VERSION.yml", "lib/bloom_filter.rb", "spec/bloom_filter_spec.rb", "ext/bloom_filter", "ext/bloom_filter/bloom.c", "ext/bloom_filter/bloom.h", "ext/bloom_filter/bloom_filter.c", "ext/bloom_filter/extconf.rb", "ext/bloom_filter/Makefile"]
  s.homepage = %q{http://github.com/tyler/bloom_filter}
  s.require_paths = ["ext", "lib"]
  s.rubygems_version = %q{1.3.1}
  s.summary = %q{Fast Bloom filter in C and Ruby.}

  if s.respond_to? :specification_version then
    current_version = Gem::Specification::CURRENT_SPECIFICATION_VERSION
    s.specification_version = 2

    if Gem::Version.new(Gem::RubyGemsVersion) >= Gem::Version.new('1.2.0') then
    else
    end
  else
  end
end
