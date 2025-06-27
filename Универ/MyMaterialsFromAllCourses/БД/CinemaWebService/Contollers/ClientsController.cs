// using CinemaWebService.Data;
// using CinemaWebService.Models;
// using Microsoft.AspNetCore.Mvc;
// using Microsoft.EntityFrameworkCore;
//
// namespace CinemaWebService.Controllers
// {
//     [ApiController]
//     [Route("api/[controller]")]
//     public class ClientsController : ControllerBase
//     {
//         private readonly ApplicationDbContext _context;
//
//         public ClientsController(ApplicationDbContext context)
//         {
//             _context = context;
//         }
//         
//         [HttpGet]
//         public async Task<ActionResult<IEnumerable<Client>>> GetAllClients()
//         {
//             return await _context.Clients.ToListAsync();
//         }
//         
//         [HttpGet("{id}")]
//         public async Task<ActionResult<Client>> GetClientById(int id)
//         {
//             var client = await _context.Clients.FindAsync(id);
//
//             if (client == null)
//             {
//                 return NotFound();
//             }
//
//             return client;
//         }
//         
//         [HttpPost]
//         public async Task<ActionResult<Client>> CreateClient(Client client)
//         {
//             _context.Clients.Add(client);
//             await _context.SaveChangesAsync();
//
//             // Возвращаем код 201 + путь к новому ресурсу
//             return CreatedAtAction(nameof(GetClientById), new { id = client.ClientId }, client);
//         }
//         
//         [HttpPut("{id}")]
//         public async Task<IActionResult> UpdateClient(int id, Client updatedClient)
//         {
//             if (id != updatedClient.ClientId)
//             {
//                 return BadRequest("Идентификаторы не совпадают");
//             }
//
//             _context.Entry(updatedClient).State = EntityState.Modified;
//
//             try
//             {
//                 await _context.SaveChangesAsync();
//             }
//             catch (DbUpdateConcurrencyException)
//             {
//                 if (!ClientExists(id))
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
//         public async Task<IActionResult> DeleteClient(int id)
//         {
//             var client = await _context.Clients.FindAsync(id);
//             if (client == null)
//             {
//                 return NotFound();
//             }
//
//             _context.Clients.Remove(client);
//             await _context.SaveChangesAsync();
//
//             return NoContent();
//         }
//
//         private bool ClientExists(int id)
//         {
//             return _context.Clients.Any(e => e.ClientId == id);
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
    public class ClientsController : ControllerBase
    {
        private readonly IMongoCollection<Client> _clientsCollection;

        public ClientsController(IMongoDatabase database)
        {
            _clientsCollection = database.GetCollection<Client>("clients");
        }
        
        [HttpGet]
        public async Task<ActionResult<IEnumerable<Client>>> GetAllClients()
        {
            var clients = await _clientsCollection.Find(_ => true).ToListAsync();
            return Ok(clients);
        }
        
        [HttpGet("{id}")]
        public async Task<ActionResult<Client>> GetClientById(int id)
        {
            var client = await _clientsCollection.Find(c => c.ClientId == id).FirstOrDefaultAsync();
            if (client == null)
            {
                return NotFound();
            }

            return Ok(client);
        }
        
        [HttpPost]
        public async Task<ActionResult<Client>> CreateClient(Client client)
        {
            var maxId = await _clientsCollection.AsQueryable().MaxAsync(c => (int?)c.ClientId) ?? 0;
            client.ClientId = maxId + 1;

            await _clientsCollection.InsertOneAsync(client);

            return CreatedAtAction(nameof(GetClientById), new { id = client.ClientId }, client);
        }
        
        [HttpPut("{id}")]
        public async Task<IActionResult> UpdateClient(int id, Client updatedClient)
        {
            if (id != updatedClient.ClientId)
            {
                return BadRequest("Идентификаторы не совпадают");
            }

            var replaceResult = await _clientsCollection.ReplaceOneAsync(c => c.ClientId == id, updatedClient);
            if (replaceResult.MatchedCount == 0)
            {
                return NotFound();
            }
            
            return NoContent();
        }
        
        [HttpDelete("{id}")]
        public async Task<IActionResult> DeleteClient(int id)
        {
            var deleteResult = await _clientsCollection.DeleteOneAsync(c => c.ClientId == id);
            if (deleteResult.DeletedCount == 0)
            {
                return NotFound();
            }
            return NoContent();
        }
    }
}