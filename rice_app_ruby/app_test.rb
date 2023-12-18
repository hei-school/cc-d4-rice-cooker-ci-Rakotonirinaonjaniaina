require 'test/unit'
require_relative 'rice_cooker'

class TestRiceCooker < Test::Unit::TestCase
  def setup
    @rice_cooker = RiceCooker.new
  end

  def test_functionalities_display
    output = capture_output { @rice_cooker.show_functionalities }
    assert(output.include?('Rice Cooker Functionalities:'), 'Functionalities should be displayed')
    assert(output.include?('1. Cooking rice'), 'Should display Cooking rice')
  end

  def test_valid_cooking_details
    details = { water_liters: 0, timer: 0 }
    assert_false(@rice_cooker.valid_cooking_details?(details), 'Should return false for invalid details')

    valid_details = { water_liters: 2, timer: 5 }
    assert(@rice_cooker.valid_cooking_details?(valid_details), 'Should return true for valid details')
  end

  def test_cook_rice_with_valid_details
    details = { water_liters: 2, timer: 1 }
    assert_nothing_raised('Should not raise error') { @rice_cooker.start_cooking(details) }
  end

  def test_guide_quantities
    persons = 4
    output = capture_output { @rice_cooker.guide_quantities(persons) }
    assert(output.include?("For #{persons} person(s), you will need:"), 'Should display quantities for persons')
  end

  def capture_output(&block)
    original_stdout = $stdout
    $stdout = fake = StringIO.new
    begin
      block.call
      fake.string
    ensure
      $stdout = original_stdout
    end
  end
end
