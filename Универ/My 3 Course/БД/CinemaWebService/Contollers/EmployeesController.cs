// using Microsoft.AspNetCore.Mvc;
// using Microsoft.EntityFrameworkCore;
// using CinemaWebService.Data;
// using CinemaWebService.Models;
//
// namespace CinemaWebService.Controllers
// {
//     [ApiController]
//     [Route("api/[controller]")]
//     public class EmployeesController : ControllerBase
//     {
//         private readonly ApplicationDbContext _context;
//
//         public EmployeesController(ApplicationDbContext context)
//         {
//             _context = context;
//         }
//         
//         [HttpGet]
//         public async Task<ActionResult<IEnumerable<Employee>>> GetEmployees()
//         {
//             return await _context.Employees
//                 // .Include(e => e.SessionEmployees)
//                 .ToListAsync();
//         }
//         
//         [HttpGet("{id}")]
//         public async Task<ActionResult<Employee>> GetEmployee(int id)
//         {
//             var employee = await _context.Employees
//                 // .Include(e => e.SessionEmployees)
//                 .FirstOrDefaultAsync(e => e.EmployeeId == id);
//
//             if (employee == null)
//             {
//                 return NotFound();
//             }
//
//             return employee;
//         }
//         
//         [HttpPost]
//         public async Task<ActionResult<Employee>> PostEmployee(Employee employee)
//         {
//             _context.Employees.Add(employee);
//             await _context.SaveChangesAsync();
//
//             return CreatedAtAction(nameof(GetEmployee), new { id = employee.EmployeeId }, employee);
//         }
//         
//         [HttpPut("{id}")]
//         public async Task<IActionResult> PutEmployee(int id, Employee employee)
//         {
//             if (id != employee.EmployeeId)
//             {
//                 return BadRequest("Идентификаторы не совпадают");
//             }
//
//             _context.Entry(employee).State = EntityState.Modified;
//
//             try
//             {
//                 await _context.SaveChangesAsync();
//             }
//             catch (DbUpdateConcurrencyException)
//             {
//                 if (!EmployeeExists(id))
//                 {
//                     return NotFound();
//                 }
//                 throw;
//             }
//
//             return NoContent();
//         }
//         
//         [HttpDelete("{id}")]
//         public async Task<IActionResult> DeleteEmployee(int id)
//         {
//             var employee = await _context.Employees.FindAsync(id);
//             if (employee == null)
//             {
//                 return NotFound();
//             }
//
//             _context.Employees.Remove(employee);
//             await _context.SaveChangesAsync();
//
//             return NoContent();
//         }
//
//         private bool EmployeeExists(int id)
//         {
//             return _context.Employees.Any(e => e.EmployeeId == id);
//         }
//     }
// }

using Microsoft.AspNetCore.Mvc;
using MongoDB.Driver;
using CinemaWebService.Models;
using MongoDB.Driver.Linq;

namespace CinemaWebService.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class EmployeesController : ControllerBase
    {
        private readonly IMongoCollection<Employee> _employeesCollection;

        public EmployeesController(IMongoDatabase database)
        {
            _employeesCollection = database.GetCollection<Employee>("employees");
        }
        
        [HttpGet]
        public async Task<ActionResult<IEnumerable<Employee>>> GetEmployees()
        {
            var employees = await _employeesCollection.Find(_ => true).ToListAsync();
            return Ok(employees);
        }
        
        [HttpGet("{id}")]
        public async Task<ActionResult<Employee>> GetEmployee(int id)
        {
            var employee = await _employeesCollection.Find(e => e.EmployeeId == id).FirstOrDefaultAsync();
            if (employee == null)
            {
                return NotFound();
            }

            return Ok(employee);
        }
        
        [HttpPost]
        public async Task<ActionResult<Employee>> PostEmployee(Employee employee)
        {
            var maxId = await _employeesCollection.AsQueryable().MaxAsync(e => (int?)e.EmployeeId) ?? 0;
            employee.EmployeeId = maxId + 1;

            await _employeesCollection.InsertOneAsync(employee);
            return CreatedAtAction(nameof(GetEmployee), new { id = employee.EmployeeId }, employee);
        }
        
        [HttpPut("{id}")]
        public async Task<IActionResult> PutEmployee(int id, Employee employee)
        {
            if (id != employee.EmployeeId)
            {
                return BadRequest("Идентификаторы не совпадают");
            }

            var replaceResult = await _employeesCollection.ReplaceOneAsync(e => e.EmployeeId == id, employee);
            if (replaceResult.MatchedCount == 0)
            {
                return NotFound();
            }

            return NoContent();
        }
        
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteEmployee(int id)
        {
            var deleteResult = await _employeesCollection.DeleteOneAsync(e => e.EmployeeId == id);
            if (deleteResult.DeletedCount == 0)
            {
                return NotFound();
            }
            return NoContent();
        }
    }
}