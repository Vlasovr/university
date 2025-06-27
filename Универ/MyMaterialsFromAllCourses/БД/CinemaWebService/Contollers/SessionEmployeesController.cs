// using Microsoft.AspNetCore.Mvc;
// using Microsoft.EntityFrameworkCore;
// using CinemaWebService.Data;
// using CinemaWebService.Models;
//
// namespace CinemaWebService.Controllers
// {
//     [ApiController]
//     [Route("api/[controller]")]
//     public class SessionEmployeesController : ControllerBase
//     {
//         private readonly ApplicationDbContext _context;
//
//         public SessionEmployeesController(ApplicationDbContext context)
//         {
//             _context = context;
//         }
//
//
//         [HttpGet]
//         public async Task<ActionResult<IEnumerable<SessionEmployeeView>>> GetAllSessionEmployees()
//         {
//             var sessionEmployees = await _context.Set<SessionEmployee>()
//                 .Include(se => se.Employee)
//                 .ToListAsync();
//             
//             var result = sessionEmployees
//                 .Select(se => new SessionEmployeeView
//                 {
//                     SessionId = se.SessionId,
//                     EmployeeId = se.EmployeeId,
//                     SessionInfo = se.SessionId.ToString(),
//                     EmployeeInfo = se.Employee?.FullName ?? string.Empty
//                 })
//                 .ToList();
//
//             return Ok(result);
//         }
//
//         [HttpGet("{sessionId}/{employeeId}")]
//         public async Task<ActionResult<SessionEmployeeView>> GetSessionEmployee(int sessionId, int employeeId)
//         {
//             var sessionEmployee = await _context.Set<SessionEmployee>()
//                 .Include(se => se.Employee)
//                 .FirstOrDefaultAsync(se =>
//                     se.SessionId == sessionId && se.EmployeeId == employeeId
//                 );
//
//             if (sessionEmployee == null)
//             {
//                 return NotFound();
//             }
//             
//             var view = new SessionEmployeeView
//             {
//                 SessionId = sessionEmployee.SessionId,
//                 EmployeeId = sessionEmployee.EmployeeId,
//                 SessionInfo = sessionEmployee.SessionId.ToString(),
//                 EmployeeInfo = sessionEmployee.Employee?.FullName ?? string.Empty
//             };
//
//             return Ok(view);
//         }
//         
//         [HttpPost]
//         public async Task<ActionResult<SessionEmployee>> CreateSessionEmployee(SessionEmployee sessionEmployee)
//         {
//             bool sessionExists = _context.Sessions.Any(s => s.SessionId == sessionEmployee.SessionId);
//             bool employeeExists = _context.Employees.Any(e => e.EmployeeId == sessionEmployee.EmployeeId);
//             if (!sessionExists || !employeeExists)
//             {
//                 return BadRequest("Указанные SessionId или EmployeeId не существуют.");
//             }
//             
//             bool alreadyExists = _context.Set<SessionEmployee>().Any(se => 
//                 se.SessionId == sessionEmployee.SessionId && se.EmployeeId == sessionEmployee.EmployeeId
//             );
//             if (alreadyExists)
//             {
//                 return Conflict("Эта связка SessionId/EmployeeId уже существует.");
//             }
//
//             _context.Set<SessionEmployee>().Add(sessionEmployee);
//             await _context.SaveChangesAsync();
//
//             return CreatedAtAction(
//                 nameof(GetSessionEmployee),
//                 new { sessionId = sessionEmployee.SessionId, employeeId = sessionEmployee.EmployeeId },
//                 sessionEmployee
//             );
//         }
//         
//         [HttpDelete("{sessionId}/{employeeId}")]
//         public async Task<IActionResult> DeleteSessionEmployee(int sessionId, int employeeId)
//         {
//             var sessionEmployee = await _context.Set<SessionEmployee>()
//                 .FindAsync(sessionId, employeeId);
//
//             if (sessionEmployee == null)
//             {
//                 return NotFound();
//             }
//
//             _context.Set<SessionEmployee>().Remove(sessionEmployee);
//             await _context.SaveChangesAsync();
//
//             return NoContent();
//         }
//     }
//     
//     public class SessionEmployeeView
//     {
//         public int SessionId { get; set; }
//         public int EmployeeId { get; set; }
//
//         public string SessionInfo { get; set; } = string.Empty;  
//         public string EmployeeInfo { get; set; } = string.Empty;
//     }
// }
//


using Microsoft.AspNetCore.Mvc;
using MongoDB.Driver;
using CinemaWebService.Models;

namespace CinemaWebService.Controllers
{
    [ApiController]
    [Route("api/[controller]")]
    public class SessionEmployeesController : ControllerBase
    {
        private readonly IMongoCollection<SessionEmployee> _sessionEmployeesCollection;
        private readonly IMongoCollection<Employee> _employeesCollection;
        private readonly IMongoCollection<Session> _sessionsCollection;

        public SessionEmployeesController(IMongoDatabase database)
        {
            // Коллекции (названия - на ваше усмотрение)
            _sessionEmployeesCollection = database.GetCollection<SessionEmployee>("sessionEmployees");
            _employeesCollection        = database.GetCollection<Employee>("employees");
            _sessionsCollection         = database.GetCollection<Session>("sessions");
        }

        [HttpGet]
        public async Task<ActionResult<IEnumerable<SessionEmployeeView>>> GetAllSessionEmployees()
        {
            // Считываем все sessionEmployee
            var sessionEmployees = await _sessionEmployeesCollection.Find(_ => true).ToListAsync();

            // Для "EmployeeInfo" нужно подтянуть Employee
            // Можно сделать дополнительный запрос в employeesCollection
            // Ниже — пример "ленивого" получения Employee.FullName
            var employees = await _employeesCollection
                .Find(_ => true).ToListAsync(); // Загрузим всех (для демо)

            // Аналогично Sessions, если хотите подтягивать SessionId -> SessionInfo
            // var sessions = await _sessionsCollection.Find(_ => true).ToListAsync();

            var result = sessionEmployees.Select(se =>
            {
                var emp = employees.FirstOrDefault(e => e.EmployeeId == se.EmployeeId);
                return new SessionEmployeeView
                {
                    SessionId    = se.SessionId,
                    EmployeeId   = se.EmployeeId,
                    SessionInfo  = se.SessionId.ToString(),
                    EmployeeInfo = emp?.FullName ?? string.Empty
                };
            }).ToList();

            return Ok(result);
        }

        [HttpGet("{sessionId}/{employeeId}")]
        public async Task<ActionResult<SessionEmployeeView>> GetSessionEmployee(int sessionId, int employeeId)
        {
            // Ищем документ
            var filter = Builders<SessionEmployee>.Filter.Eq(se => se.SessionId, sessionId) &
                         Builders<SessionEmployee>.Filter.Eq(se => se.EmployeeId, employeeId);

            var sessionEmployee = await _sessionEmployeesCollection.Find(filter).FirstOrDefaultAsync();
            if (sessionEmployee == null)
            {
                return NotFound();
            }

            // Подтянем Employee
            var employee = await _employeesCollection
                .Find(e => e.EmployeeId == sessionEmployee.EmployeeId).FirstOrDefaultAsync();

            var view = new SessionEmployeeView
            {
                SessionId    = sessionEmployee.SessionId,
                EmployeeId   = sessionEmployee.EmployeeId,
                SessionInfo  = sessionEmployee.SessionId.ToString(),
                EmployeeInfo = employee?.FullName ?? string.Empty
            };

            return Ok(view);
        }

        [HttpPost]
        public async Task<ActionResult<SessionEmployee>> CreateSessionEmployee(SessionEmployee sessionEmployee)
        {
            // Проверка на существование Session и Employee
            var sessionExists = await _sessionsCollection
                .Find(s => s.SessionId == sessionEmployee.SessionId).AnyAsync();

            var employeeExists = await _employeesCollection
                .Find(e => e.EmployeeId == sessionEmployee.EmployeeId).AnyAsync();

            if (!sessionExists || !employeeExists)
            {
                return BadRequest("Указанные SessionId или EmployeeId не существуют.");
            }

            // Проверяем, что связка не дублируется
            var filter = Builders<SessionEmployee>.Filter.Eq(se => se.SessionId, sessionEmployee.SessionId) &
                         Builders<SessionEmployee>.Filter.Eq(se => se.EmployeeId, sessionEmployee.EmployeeId);

            bool alreadyExists = await _sessionEmployeesCollection.Find(filter).AnyAsync();
            if (alreadyExists)
            {
                return Conflict("Эта связка SessionId/EmployeeId уже существует.");
            }

            await _sessionEmployeesCollection.InsertOneAsync(sessionEmployee);

            return CreatedAtAction(
                nameof(GetSessionEmployee),
                new { sessionId = sessionEmployee.SessionId, employeeId = sessionEmployee.EmployeeId },
                sessionEmployee
            );
        }

        [HttpDelete("{sessionId}/{employeeId}")]
        public async Task<IActionResult> DeleteSessionEmployee(int sessionId, int employeeId)
        {
            var filter = Builders<SessionEmployee>.Filter.Eq(se => se.SessionId, sessionId) &
                         Builders<SessionEmployee>.Filter.Eq(se => se.EmployeeId, employeeId);

            var deleteResult = await _sessionEmployeesCollection.DeleteOneAsync(filter);

            if (deleteResult.DeletedCount == 0)
            {
                return NotFound();
            }

            return NoContent();
        }
    }
    
    public class SessionEmployeeView
    {
        public int SessionId { get; set; }
        public int EmployeeId { get; set; }

        public string SessionInfo { get; set; } = string.Empty;  
        public string EmployeeInfo { get; set; } = string.Empty;
    }
}